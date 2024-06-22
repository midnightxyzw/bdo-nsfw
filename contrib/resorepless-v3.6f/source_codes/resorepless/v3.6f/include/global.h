
#ifndef GLOBAL_H
#define GLOBAL_H
#include <stdlib.h>
#include <stdio.h>

#define NUM_THREADS 3

/** IMPORTANT STUFF **/
#define RANDOM_FOLDER_NUM 1
#define RANDOM_FILE_NUM 60556

#define ONE_REGISTRY 28
#define TOTAL_CLASSES 16
#define TOTAL_FEMALE_CLASSES 10
#define TOTAL_PHW_CLASSES 4

#define TOTAL_MALE_CLASSES 6
#define MAX_TEXTURES 512

#define TOTAL_TEXTURE_VARIATIONS 4
#define HELMET_COUNT 395

#define TOTAL_NUDE_FEMALE_TEXTURES 5
#define TOTAL_FEMALE_NORMAL_MAPS_TEXTURES 4
#define TOTAL_NUDE_MALE_TEXTURES 6

#define TOTAL_CENSORSHIP_FILES 25

#define TOTAL_PANTIES 53
#define TOTAL_PANTS 37
#define TOTAL_STOCKINGS 20
#define TOTAL_STARTER_STOCKINGS 9
#define TOTAL_TREEE_DUMMER_FILES 8


#define CUSTOMIZE_MENU_N_OPTIONS 13
#define BODY_SIZE_N_OPTIONS 7


#define NONE 0
#define NORMAL 1
#define HARD 2


/** KEYBOARD KEYS MACROS */
#define NOT_FOUND -1
#define ENTER 13
#define SPACE 32
#define PAGE_UP 73
#define PAGE_DOWN 81
#define BACKSPACE 8
#define ARROW_UP 72
#define ARROW_DOWN 80
#define ARROW_LEFT 75
#define ARROW_RIGHT 77
#define ESC 27
#define F1 59
#define F2 60
#define F3 61
#define F4 62
#define F5 63
#define F6 64
#define F7 65
#define F8 66
#define F9 67
#define F10 68


/** RETURN VALUES */
#define SUCCESS 1
#define FAILURE 0

#define EXIT 0
#define BACK 0

#define ON 1
#define OFF 0


/** MAIN MENU OPTIONS */
#define INSTALL 10
#define CUSTOMIZE 2
#define CHANGE_SIZES 3
#define KIBELIUS 4
#define TOOLS 5
#define UNINSTALL 6
#define LIST_PATCHED_FILES 7


#define RESTORE_BACKUP 1
#define UNDO 2

#define REMOVE 2
#define RESTORE 3

#define BROWSE_GAME_FILES 1
#define REMOVE_A_FILE 2
#define RESTORE_A_FILE -11
#define FIX_HOLES 3
#define GET_TEXTURES 4
#define GET_FILES_FROM_TEXTURE 5

#define BROWSE_FILES 1
#define MANUAL_INSERT 2
#define PATCH_FROM_FILE 3


/** KIBELIUS ARMOR */
#define TRANSPARENT 1
#define MORE_TRANSPARENT 2
#define NAKED_WITH_WINGS 3

#define KEEP_WINGS 1
#define REMOVE_WINGS 0

extern int isWindows10;


extern long filesPatched;
extern int FLAG;
extern int displayedErrorOnce;

extern char* LATEST_MODIFICATIONS_FILE_NAME;
extern char* PATCHER_STATE_FILE_NAME;
extern char* PAZ_BROWSER_PATCHER_STATE_FILE_NAME[];
extern char* PAZ_BROWSER_RED_MARKERS_FILE_NAME[];

extern float MAX_DEFAULT_SIZE;
extern float MIN_DEFAULT_SIZE;
extern float DEFAULT_DEFAULT_SIZE;


/** PUBIC HAIR PATCHER STATE IDX MACROS **/
typedef enum {RANGER_PUBIC_HAIR,WITCH_PUBIC_HAIR,TAMER_PUBIC_HAIR,DARK_KNIGHT_PUBIC_HAIR,SORCERESS_PUBIC_HAIR,VALKYRIE_PUBIC_HAIR,MAEHWA_PUBIC_HAIR,KUNOICHI_PUBIC_HAIR,MYSTIC_PUBIC_HAIR} PubicHairClass;

typedef enum {WARRIOR_PENIS,BERSERKER_PENIS,BLADER_PENIS,WIZARD_PENIS,NINJA_PENIS,STRIKER_PENIS} PenisClass;

typedef enum {ARMOR,UNDERWEAR,LOWERBODY,GLOVES,BOOTS,HELMETS,STOCKINGS,LIFE_SKILL_ARMORS,GENDER,NPC,USE_3D_VAGINAS,USE_PENISES} PatcherOption;

#define SHOULDER 9
#define CLOAK 10
#define WEAPONS 92

typedef enum {SHAVED,SHAVED_INNIE,FULL_BUSH,FULL_BUSH_2,FULL_BUSH_3,MEDIUM_BUSH,MEDIUM_BUSH_2,SMALL_BUSH,SMALL_BUSH_2,THIN_LANDING_STRIP,WIDE_LANDING_STRIP,TRIMMED,WIDER_TRIMMED} PubicHairOption;
typedef enum {BREASTS,BUTT,THIGHS,ARMS,LEGS,PELVIS,SPINE} BodyPartSizeOption;

typedef enum {REMOVE_ALL,KEEP_ALL,STARTER,REGULAR,PANTIES_ONLY,IN} Restriction;

typedef enum {WARRIOR,SORCERESS,RANGER,BERSERKER,TAMER,BLADER,VALKYRIE,WIZARD,WITCH,KUNOICHI,NINJA,DARK_KNIGHT,MAEHWA,STRIKER,MYSTIC,RAN} Class;

typedef enum {PHM,PHW,PEW,PGM,PBW,PKM,PVW,PWM,PWW,PNW,PNM,PDW,PKWW,PCM,PCW} ClassPrefix;

extern char *starters[][TOTAL_CLASSES];
extern char *weaponsFolder[][4];
extern char* panties[];
extern char* pants[];
extern char* censorshipTextureFiles[];
extern char* starterStockings[];
extern char* starterUnderwearTextures[];

extern char* nudeFemaleTextures[];
extern char* nudeFemaleNormalMaps[];
extern char* nudeFemaleModels[];

extern char* sorceressSecundaryNudeTextures[];

extern char* nudeMaleTextures[];
extern char* nudeMaleModels[];

extern char* pubicHairFolder[];

extern int classRestrictions[TOTAL_CLASSES];
extern char* cutExceptionList[];
extern char * stockings[];

extern char* treedummer_files[];



#define REMOVE_ALL_ARMORS 0
#define KEEP_ALL_ARMORS 1
#define REMOVE_STARTER_ARMORS 2
#define REMOVE_REGULAR_ARMORS 3


#define REMOVE_ALL_UNDERWEAR 0
#define KEEP_ALL_UNDERWEAR 1
#define REMOVE_STARTER_UNDERWEAR 2

#define CENSORSHIP 2

#define KEEP_CENSORSHIP 0
#define MINIMAL_CENSORSHIP_REMOVAL 1
#define MEDIUM_CENSORSHIP_REMOVAL 2
#define HIGH_CENSORSHIP_REMOVAL 3


#define REMOVE_ALL_GLOVES 0
#define KEEP_ALL_GLOVES 1
#define REMOVE_STARTER_GLOVES 2
#define REMOVE_REGULAR_GLOVES 3


#define REMOVE_ALL_BOOTS 0
#define KEEP_ALL_BOOTS 1
#define REMOVE_STARTER_BOOTS 2
#define REMOVE_REGULAR_BOOTS 3


#define REMOVE_ALL_HELMETS 1
#define KEEP_ALL_HELMETS 0

#define REMOVE_ALL_STOCKINGS 0
#define KEEP_ALL_STOCKINGS 1
#define REMOVE_STARTER_STOCKINGS 2


#define REMOVE_ALL_LIFE_SKILL 0
#define KEEP_ALL_LIFE_SKILL 1

#define REMOVE_NPC_CLOTHING 0



#define FEMALE_ONLY 0
#define MALE_ONLY 1
#define BOTH_GENDERS 2

#define PRIMARY_WEAPON 2
#define SECUNDARY_WEAPON 3
#define BOTH_WEAPONS 4
#define AWAKENING_WEAPON 5

#define PRIMARY_WEAPON_FOLDER 0
#define SECUNDARY_WEAPON_FOLDER 1
#define AWAKENING_WEAPON_FOLDER 2



#endif // GLOBAL_H
