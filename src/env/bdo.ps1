"********************************************************************************"
"*                                                                              *"
"*                        BDO Mod Development Console                           *"
"*                                                                              *"
"********************************************************************************"

# ==============================================================================
# Define local functions
# ==============================================================================

function warn { write-host -ForegroundColor yellow "WARN : $args" }

function error {
    write-host -ForegroundColor red "BDO Mod build environment setup failed : $args"
    stop-process $PID # kill current power shell process
}

function catch_batch_env( $batch, $arg )
{
    $tag = "[[[===start-environment-table===]]]" # cannot have space here.
    $cmd = "`"$batch`" $arg &echo $tag& set"
    $tag_found = $false
    cmd.exe /c $cmd|foreach-object{
        if( $tag_found )
        {
            $p, $v = $_.split('=')
            Set-Item -path env:$p -value $v
        }
        elseif( $_ -eq $tag )
        {
            $tag_found = $true
        }
        else
        {
            $_
        }
    }
}

# ==============================================================================
# Define global functions
# ==============================================================================

# Define your function like this: function global:<name> (...) { .... }
function global:ccc { cmd.exe /c $args }

# A helper function to retrieve current git branch
function global:get-git-branch {
    $branch = $(git rev-parse --abbrev-ref HEAD 2>&1)
    if ($lastExitCode -ne 0) {
        $branch = "!!!GIT ERROR: {$lastExitCode}!!!"
    }
    "$branch"
}

# redefine prompt function
function global:prompt {
    write-host -ForegroundColor Green "==== BDO Mod - " -NoNewline
    write-host -ForegroundColor Blue "$BDO_MOD_ROOT" -NoNewline
    write-host -ForegroundColor Green " - " -NoNewline
    write-host -ForegroundColor Yellow "$(get-git-branch)" -NoNewline
    write-host -ForegroundColor Green " ===="
    write-host -ForegroundColor Green "[$(get-location)]"
    return ">"
}

# ==============================================================================
# Get the root directory
# ==============================================================================

# note: $BDO_MOD_ROOT is a global variable that could be used in other places outside of this script.
write-host "Detecting repository root directory..."
$global:BDO_MOD_ROOT=split-path -parent $PSScriptRoot | split-path -parent
$env:BDO_MOD_ROOT=$BDO_MOD_ROOT

# ==============================================================================
# Check Python support
# ==============================================================================

$py = [System.Version]$(python.exe -V).Substring(7)
if ([System.Version]"3.6.0" -gt $py) {
    warn "Python is not installed or current version ($py) is too low. Please upgrade to 3.8.0+ for best script compatibility."
}
write-host "Python version: $py found."

# Setup Virtual Python Environment
$python_venv_path = "$BDO_MOD_ROOT\src\.python_venv"
if (-not (Test-Path -path "$python_venv_path\Scripts\activate.ps1")) {
    write-host "Initializing virtual python environment at $python_venv_path ..."
    python -m venv $python_venv_path
}
write-host "Activating virtual python environment at $python_venv_path ..."
. "$python_venv_path\Scripts\activate.ps1"
python -m pip install --upgrade pip
python -m pip install -r "$BDO_MOD_ROOT\src\requirements.txt"

# ==============================================================================
# setup aliases
# ==============================================================================
write-host "Setting up aliases..."
if( Test-Path -path "$BDO_MOD_ROOT\src\env\alias.powershell.txt" )
{
    # create script block for all aliases
    $aliases = ""
    get-content "$BDO_MOD_ROOT\src\env\alias.powershell.txt"|foreach {
        $name, $value = $_.split(' ')
        $body = ([System.String]$value).Trim( ' "' )
        $aliases = $aliases +
        "
        function global:$name {$body}
        "
    }
    $aliases = $executioncontext.InvokeCommand.NewScriptBlock( $aliases )

    # run the script
    &$aliases
} else {
    warn "No alias file found at $BDO_MOD_ROOT\src\env\alias.powershell.txt"
}

# ==============================================================================
# setup git
# ==============================================================================
write-host "Setting up git..."
if ( Test-Path -path "$BDO_MOD_ROOT\.gitconfig" )
{
    git config --local include.path ${BDO_MOD_ROOT}/.gitconfig
}

# ==============================================================================
# MISC
# ==============================================================================

# Setup PATH
write-host "Setting up PATH..."
$env:Path = "$BDO_MOD_ROOT\src\bin;$env:Path"

# update title
write-host "Setting up window title..."
$Host.UI.RawUI.WindowTitle = "bdo-mod ( $BDO_MOD_ROOT )"

# change current location
set-location $BDO_MOD_ROOT

#reset some command line color
Set-PSReadlineOption -Colors @{
    "Parameter" = [System.ConsoleColor]::White
    "Operator" = [System.ConsoleColor]::White
}

# ==============================================================================
# DONE
# ==============================================================================

write-host -ForegroundColor green "

BDO_MOD_ROOT  = $env:BDO_MOD_ROOT
USERNAME       = $env:USERNAME

BDO Mod development environment is ready to use. Happy coding!
"
write-host -ForegroundColor yellow "
Run ""format-all-sources.sh"" to to clean up your code style.
"
