param(
    [Switch]$clear,

    [ValidateSet('F', "M", "B")]
    $gender,

    [ValidateSet('P', "F", "A")]
    $armor
)

function rip {
    param(
        $message
    )
    write-host $message
    exit -1
}

function warning {
    param($message)
    write-host "[WARNING] $message"
}

function check_folder {
    param($folder)
    if (-not (test-path -Path $folder -PathType container)) {
        rip("$folder not found. Your mod files seems corrupted.")
    }
}

function remove_items_with_substring {
    param (
        [string[]]$List,
        [string]$Substring
    )

    # write-host "list = $List"
    # write-host "Substring = $Substring"

    # Create a new list to store the items that do not contain the substring
    $filteredList = @()

    # Loop through each item in the list
    foreach ($item in $List) {
        # Check if the item contains the substring
        # write-host "  Check $item against $SubString..."
        if ($item -notlike "*$Substring*") {
            # If it does not contain the substring, add it to the filtered list
            $filteredList += $item
            # write-host "    Not Match. This is a keeper."
        } else {
            # write-host "    Match. Ignore it."
        }
    }

    # Return the filtered list
    return $filteredList
}

function remove_mods {
    param ($folder)
    check_folder($folder)
    $mods = get-childitem -Path $folder -Directory
    foreach($m in $mods) {
        try {
            Remove-Item -Path "$folder/$m" -Force -Recurse 
            write-host "  Removed mod: $($m)"
        } catch {
            rip "Failed to remove mod folder: $($m). Error: $_"
        }
    }
}

function clean_up_exsiting_mods {
    write-host ""
    write-host "Cleanup existing mods in ""$dest_folder""..."
    remove_mods "${dest_folder}"
}

# print a shorter version of the path
function shorter {
    param (
        $fullPath
    )
    $newString = $fullPath.Replace("$pazDir/", "")
    return $newString
}

function deploy_mod {
    param ($source, $dest)
    write-host ""
    write-host "  Install Mod:"
    # write-host "    source: $source"
    # write-host "      dest: $dest"
    $dest_parent = split-path -Path $dest -Parent
    # write-host "    create folder: $dest_parent"
    write-host "    from: $(shorter $source)"
    write-host "      to: $(shorter $dest)"
    # assign to dummy variable to silent the stdout of the command.
    $dummy = mkdir $dest_parent -Force
    $dummy = cmd.exe /C "mklink /D ""$dest"" ""$source"""
}

# print welcome message
write-host ""
write-host "Welcome to Midnight Xyzw mod. This script will customize the mod based on your selection."
write-host ""

# initialize key folder variabes
$pazDir = split-path -path $PSScriptRoot -parent
$source_folder="${pazDir}/.midnight_xyzw"
$dest_folder = "${pazDir}/files_to_patch/_midnight_xyzw"

# check if it is clean up code.
if ($clear) {
    clean_up_exsiting_mods
    exit 0
}

# Prompt for missing arguments
if (-not $gender) {
    $gender = ($(read-host -Prompt "Which gender's armor you want to hide? F(emale), M(ale) or B(oth)").toupper())
}
$valid_gender_options = @("F", "M", "B")
if(-not ($valid_gender_options -contains $gender)) {
    rip("Invalid gender selection. Must be oene of $($valid_gender_options -join ', ')")
}
if (-not $armor) {
    $armor = ($(read-host -Prompt "What kind of armors you want to hide? P(earl), F(ree) or A(all)").toupper())
}
$valid_armor_options = @("P", "F", "A")
if(-not ($valid_armor_options -contains $armor)) {
    rip("Invalid armor selection. Must be one of: $($valid_armor_options -join ', ')")
}

# Give user a chance to double confirm the seletion.
write-host ""
write-host "Your current selection is"
write-host "  Gender = $gender"
write-host "  Armor  = $armor"
write-host ""
$continue = read-host "Press Enter if you wish to continue, or any other key to stop"
if ("" -ne $continue) {
    write-host "Stopped."
    exit 1
}

# All user selections validated and confirmed. Now let's do business.

# Collect list of customizable folders
$mods = @(
    "_00_suzu_nude",
    "_00_npc_and_monster"
)
check_folder "$source_folder"
foreach ($mod in @("_00_remove_all_armors", "_00_remove_underwear")) {
    $modDir = "${source_folder}/$mod"
    if (-not (test-path -Path $modDir -PathType container)) {
        warning "$modDir not found. Skipped."
        continue
    }
    $categories = get-childitem -Path $modDir -Directory
    if ($categories.Length -eq 0) {
        warning "$d is empty. Corrupted mod files?"
        continue
    }
    foreach($c in $categories) {
        $mods += "$mod/${c}"
    }
}
# "initial mod list = $mods"

# Generate symbolic links in file_to_patch folder.
if ("F" -eq $gender) {
    # User wants female only mods. Remove male mods from the list.
    $mods = remove_items_with_substring -List $mods -Substring "_male"
}
elseif ("M" -eq $gender) {
    $mods = remove_items_with_substring -List $mods -Substring "_female"
}
if ("P" -eq $armor) {
    $mods = remove_items_with_substring -List $mods -Substring "_free"
}
elseif ("F" -eq $armor) {
    write-host "Disable pearl outfit mods..."
    $mods = remove_items_with_substring -List $mods -Substring "_pearl"
}
write-host""
write-host "Mod list to be installed:"
$mods | foreach-object { write-host "  $_"}

# clean up existing mods
clean_up_exsiting_mods

# Create symbolic link for each mod folder
write-host ""
write-host "Setup new mods..."
# write-host "  source = $source_folder"
# write-host "    dest = $dest_folder"
foreach($mod in $mods) {
    deploy_mod -dest "$dest_folder/$mod" -source "${source_folder}/$mod"
}

# the end
write-host ""
write-host "All done."