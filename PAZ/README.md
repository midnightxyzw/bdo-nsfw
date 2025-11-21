# Overview

BDO NSFW mod generated & collected by Midnight Xyzw.

# What's included

## 1. [No Underwear](files_to_patch/_player/_00_remove_underwear)
This mod hides underwear for all classes (except Shai), up to Deadeye. It does not contain any nude texture/mesh in it, to make it easier to use it with your own version of nude mod.

## 2. [Suzu's Nude Mod](files_to_patch/_player/_00_suzu_nude)
This is nude model and skin for all female classes (except Shai), up to Deadeye, originally created by fantastic [Suzu](https://www.undertow.club/downloads/authors/suzu-%E9%88%B4.26775/). This is the fixed/improved version from Discord user [TheGreateSage](https://discord.com/channels/524568440254627850/1313857998644314122)

This folder, with `No Underwear` together, provides the basis of the entire MOD.

## 3. [All Armor Removed](files_to_patch/_player/_00_remove_all_armors)

This mod hides all armors for all classes (except Shai), up to Deadeye. It has 2 options to allow you to customize which subset of outfits/armors to remove/hide from the game:

  - option 1: Female only/Male only/Both genders
  - option 2: Free items only/Pearl items only/Underwear Only/Everything.

So total 12 combinations for you to choose from. The Free VS. Pearl item separation is not perfect but should work in majority of cases

# How to use it

[**Note**] The script is only tested on original unmodified game. Run it at your own risk, if your game is already patched.

## The recommended way

This way is much faster since it utilize symbolic links to avoid file copy. But it may require you to go through a few more steps.

1. Install Python if you haven't done so. The easier way to install it on modern Windows is via `winget`:
   1. Press "Widnows + R" key to open the "Run" dialog
   2. Run: `winget install python` to install python environment. 
2. Enable Developer Mode via Windows Settings Dialog if you haven't done so. This allows you to create symbolic links as regular user.
3. If you are familiar with running scripts via command line, then:
   1. Open command line window and change to the folder where this README file is.
   2. Run `midnight_xyzw.cmd <your_bdo_paz_folder>` to deploy the mod. (`midnight_xyzw.cmd --help` for usage details)
4. If you are not used to command line:
   1. Open 2 file explorer windows. Make them side by side. So you can see both of them.
   2. One windows points to this folder where this README file and midnight_xyzw.cmd are.
   3. The other windows points to your BDO game's main folder (the parent folder of the PAZ folder)
   4. Drag the PAZ folder, drop it onto the `midnight_xyzw.cmd`. This will launch the deployment script.
   5. Follow the screen instructions to install the mod.
5. When the script finishes, it should generate a `files_to_patch` folder in your game's PAZ folder. If your game already has the `files_to_patch` folder with your own mods in it. Rest assure that the script will reuse it and not delete/modify anything of your own mods in it.
6. From the game's PAZ folder, do the following steps to patch the game:
   1. This is the chance to add additional mods to the `files_to_patch` folder if you like.
   2. Run [PartCutGen.exe](PartCutGen.exe) and follow screen instructions to refresh the part cut exclusion list.
   3. Run [Meta Injector.exe](<Meta Injector.exe>) and follow screen instructions to patch your game.
7. Have Fun

## The KISS way

This way contains less steps and could be easier to follow for non programmers. But it is slower since it involves copying & deleting large amount of files.

1. First, you still need to install Python environment, as described above. You only need to do it once though.
2. Copy everything in this folder to your game's PAZ folder.
3. From the game's PAZ folder, do the following to patch the game:
   1. Run [midnight_xyzw.cmd](midnight_xyzw.cmd) and follow screen instructions to customize the mod.
   2. Copy your own mods into the `files_to_patch` folder. This step is optional.
   3. Run [PartCutGen.exe](PartCutGen.exe) and follow screen instructions to refresh the part cut exclusion list.
   4. Run [Meta Injector.exe](<Meta Injector.exe>) and follow screen instructions to patch your game.
4. Have Fun!

# Want to contribute?

Just submit your PR if you like. Or feel free to fork and modify it however you like.

# 0.3.1 Release Notes (TBD: do not merge)

- New mod for Guardian's Kharoxia outfit
- Removed patch to all "underup" files. BDO has them removed from the game already.
- Updated pactool to v1.4.4
- Updated 3d converter to 11.102
- 

# 0.3.0 Release Nodes (12/28/2024)

Verified against 12/28/2024 official patch on NA server. Compatible with the new Deadeye class.

- What's New:
  - Generate nude and armor removal mod to the new Deadeye class.
  - Added some character/outfit specific mods in _01_xyzw_collection
- Know Issue:
  - dk grandfather crons helper has injector errors. might not work. 
  - Maegu's Crimson Flame has missing body parts

# 0.2.1.preview Release Note (07/04/2024)

Verified against 07/03/2024 official patch on NA server. Compatible with the new Dosa class

- What's New:
  - Rewrite PAZ installation script using python.
  - Added 2 independent switches to control which subset of armors to hide:
    - switch #1: Female VS. male
    - switch #2: Free VS. Pearl Items.
- Know Issues:
  - Pearl vs. free outfits separation works mostly fine, with a few exceptions. I have found that the following pearl outfits are somehow categorized as free items: Bern, Blanchard, Binari. I'm sure there are more like that. It probably won't get fixed in near future, since I'll need the exact file names of the problematic outfit for all classes, which I don't have.
  - Ranger:
    - Slight mismatch of head and body skin color.

# v0.2.0 Release Notes (06/26/2024)

- Verified against 06/26/2024 official patch on NA server.

- What's New:
  - Removed resorpless 3.6f nude mod. Replaced it with brand new script generated armor removal mod that supports all current classes and is compatible with PartCutGen.exe
  - Removed hard coded partcutdesc.xml. Now it is requires to run PartCutGen.exe before patching.
  - Woosa
    - Underwear mesh/texture removed. Will show nude in underwear mode.
  - Maegu
    - Underwear mesh/texture removed. Will show nude in underwear mode.
  - Ranger:
    - Fixed missing body part in some cash shop outfits.
  - Sorceress:
    - Fixed missing pelvis mesh.
  - Nova:
    - Fixed shadows on body
  - Mystic:
    - Fixed shadows on body

- Known Issues:
  - Dark Knight:
    - Body part missing when wearing Darkborne Rose custom.

# v0.1.0 Release Notes (06/23/2024)

- Verified against 06/19/2024 official patch of NA game server.

- Contains nude mod based on resorpless 3.6f for the following classes:
  - Sorceress
  - Ranger
  - Tamer
  - Valkyrie
  - Witch
  - Kunoichi
  - Dark Knight
  - Maehwa
  - Striker
  - Mystic
  - Lahn

- Contains Suzu's nude mod of the following classes:
  - Guardian
  - Nova
  - Maehwa
  - Corsair
  - Drakania
  - Scholar

- Not support for the following classes yet:
  - Woosa
  - Maegu

- Contains some NPC and fairy nude mod. Origin is unknown.
  - If you recognize these mods, please let me know.

- Known Issues:
  - Ranger body part missing when wearing some cash shop armor. Looks fine in underwear mode.
  - Darknight body part missing when wearing some cash shop armor. Looks fine in underwear mode.
  - Sorceress body missed one pelvis mesh. could be incorrect partcut.
  - Nova:
    - Skin has dark marks when wearing cash shop armor. Looks normal when wearing underwear only.
  - Mystic:
    - Skin has dark marks when wearing cash shop armor. Looks normal when wearing underwear only.
  - Scholar:
    - No nude armor yet. You have to be in town and enable underwear mode to see the nude mod.
  - Strange shadow on jiggled body parts. This is known to all nude mesh mods you can find online.
