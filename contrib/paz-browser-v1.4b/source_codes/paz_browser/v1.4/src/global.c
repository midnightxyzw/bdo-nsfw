#include "../include/global.h"

// is the variable that contains the 8 bytes that are going to be replaced after each reference byte is found
//int MODDED_BYTES[][2] = {{1,66404},{1,170861},{1,60556},{1,91771}};
//char *MODDED_BYTES[4] = {"\x01\x00\x00\x00\x64\x03\x01\x00","\x01\x00\x00\x00\x6D\x9B\x02\x00","\x01\x00\x00\x00\x8D\xEC\x00\x00","\x01\x00\x00\x00\x7B\x66\x01\x00"};
//                                  NA                                 RU                                   KR                               JP


long filesPatched = 0;

int ECHO = 0;
int ECHO2 = 0;

int isWindows10 = 0;

char* LATEST_MODIFICATIONS_FILE_NAME = "resorepless_latest_modifications.bin";
char* PATCHER_STATE_FILE_NAME = "resorepless_patcher_state.bin";

char* PAZ_BROWSER_PATCHER_STATE_FILE_NAME[] =
{
"",
"",
"removed_armors.txt",
"restored_armors.txt",
"partcutdesc_removed_armors.txt",
""
"",
};
char* PAZ_BROWSER_RED_MARKERS_FILE_NAME[] =
{
"",
"",
"red_markers_removed_armors.bin",
"red_markers_restored_armors.bin",
"red_markers_partcutdesc.bin",
""
"",
};

float MAX_DEFAULT_SIZE = 1.25;
float MIN_DEFAULT_SIZE = 0.90;
float DEFAULT_DEFAULT_SIZE = 1.00;



int classRestrictions[TOTAL_CLASSES] = {0};

    char *starters[][TOTAL_CLASSES] =
    {
/*ARMOR*/        {"phw_02_ub_0001.pac","pew_02_ub_0001.pac","pvw_02_ub_0007.pac","pbw_02_ub_0001.pac","pww_03_ub_0003.pac","phm_02_ub_0001.pac","pgm_02_ub_0001.pac","pkww_02_ub_0001.pac","pkm_02_ub_0001.pac","pnw_02_ub_0001.pac","pnm_02_ub_0001.pac","pdw_02_ub_0001.pac","pwm_03_ub_0003.pac","pcm_02_ub_0001.pac"},
/*UNDERWEAR*/    {"phm_00_uw_0001.pac","phw_00_uw_0001.pac","pew_00_uw_0001.pac","pgm_00_uw_0001.pac","pbw_00_uw_0001.pac","pkm_00_uw_0001.pac","pvw_00_uw_0001.pac","pwm_00_uw_0001.pac","pww_00_uw_0001.pac","pnm_00_uw_0001.pac","pnw_00_uw_0001.pac","pdw_00_uw_0001.pac", "pkww_00_uw_0001.pac", "pcm_00_uw_0001.pac"},
/*LOWERBODY*/    {"phw_02_lb_0001.pac","pew_02_lb_0001.pac","pvw_02_lb_0007.pac","pbw_02_lb_0001.pac","pww_03_lb_0003.pac","phm_02_lb_0001.pac","pgm_02_lb_0001.pac","pkww_02_lb_0001.pac","pkm_02_lb_0001.pac","pnw_02_lb_0001.pac","pnm_02_lb_0001.pac","pdw_02_lb_0001.pac","pwm_03_lb_0003.pac","pcm_02_lb_0001.pac"},
/*GLOVES*/       {"phw_02_hand_0001.pac","pew_02_hand_0001.pac","pvw_02_hand_0007.pac","pbw_02_hand_0001.pac","pww_03_hand_0003.pac","phm_02_hand_0001.pac","pgm_02_hand_0001.pac","pkww_02_hand_0001.pac","pkm_02_hand_0001.pac","pnw_02_hand_0001.pac","pnm_02_hand_0001.pac","pdw_02_hand_0001.pac","pwm_03_hand_0003.pac","pcm_02_hand_0001.pac"},
/*BOOTS*/        {"phw_02_foot_0001.pac","pew_02_foot_0001.pac","pvw_02_foot_0007.pac","pbw_02_foot_0001.pac","pww_03_foot_0003.pac","phm_02_foot_0001.pac","pgm_02_foot_0001.pac","pkww_02_foot_0001.pac","pkm_02_foot_0001.pac","pnw_02_foot_0001.pac","pnm_02_foot_0001.pac","pdw_02_foot_0001.pac","pwm_03_foot_0003.pac","pcm_02_foot_0001.pac"},
/*HELMETS*/      {"","","","","","","","","","","","","",""},
/*STOCKINGS*/    {"","","","","","","","","","","","","",""},
/*SORC_WEAPONS*/ {"phw_00_eca_0001.pac","phw_00_eca_0001_in.pac","phw_00_tlm_0001.pac","phw_00_tlm_0001_in.pac","","","","","","","","","",""},
/*GENDER*/       {"","","","","","","","","","","","","",""},
/*SHOULDER*/     {"phw_02_sho_0001.pac","","","","","","","","","","","","",""},
/*CLOAK*/        {"","","","","","","","","","","","","",""}
    };

    char *weaponsFolder[][4] =
    {
/*WARRIOR*/      {"character/model/1_pc/1_phm/weapon/1_onehandsword/","character/model/1_pc/1_phm/weapon/8_shield/","character/model/1_pc/1_phm/weapon/3_twohandsword/",""},
/*SORCERESS*/    {"character/model/1_pc/2_phw/weapon/28_enchantarm/","character/model/1_pc/2_phw/weapon/33_talisman/","character/model/1_pc/2_phw/weapon/38_scythe/",""},
/*RANGER*/       {"character/model/1_pc/3_pew/weapon/31_onehandbow/","character/model/1_pc/3_pew/weapon/32_dagger/","character/model/1_pc/3_pew/weapon/43_twinsword/",""},
/*BERSERKER*/    {"character/model/1_pc/4_pgm/weapon/29_doubleax/","character/model/1_pc/4_pgm/weapon/34_aiguillette/","character/model/1_pc/4_pgm/weapon/42_handcanon/",""},
/*TAMER*/        {"character/model/1_pc/5_pbw/weapon/30_elblade/","character/model/1_pc/5_pbw/weapon/37_norigae/","character/model/1_pc/5_pbw/weapon/44_stick/",""},
/*BLADER*/       {"character/model/1_pc/6_pkm/weapon/35_blade/","character/model/1_pc/6_pkm/weapon/36_shotbow/","character/model/1_pc/6_pkm/weapon/47_glaive/",""},
/*VALKYRIE*/     {"character/model/1_pc/7_pvw/weapon/1_onehandsword/","character/model/1_pc/7_pvw/weapon/8_shield/","character/model/1_pc/7_pvw/weapon/45_spear/",""},
/*WIZARD*/       {"character/model/1_pc/8_pwm/weapon/6_staff/","character/model/1_pc/8_pwm/weapon/32_dagger/","character/model/1_pc/8_pwm/weapon/59_orb/",""},
/*WITCH*/        {"character/model/1_pc/8_pww/weapon/6_staff/","character/model/1_pc/8_pww/weapon/32_dagger/","character/model/1_pc/8_pww/weapon/59_orb/",""},
/*NINJA*/        {"character/model/1_pc/13_pnw/weapon/30_elblade/","character/model/1_pc/13_pnw/weapon/55_shuriken/","character/model/1_pc/13_pnw/weapon/57_chakram/",""},
/*KUNOICHI*/     {"character/model/1_pc/13_pnm/weapon/30_elblade/","character/model/1_pc/13_pnm/weapon/55_shuriken/","/character/model/1_pc/13_pnm/weapon/58_katana/",""},
/*DARK_KNIGHT*/  {"character/model/1_pc/15_pdw/weapon/34_aiguillette/","character/model/1_pc/15_pdw/weapon/63_longblade/","character/model/1_pc/15_pdw/weapon/64_darksword/",""},
/*PLUM*/         {"character/model/1_pc/22_pkww/weapon/35_blade/","character/model/1_pc/22_pkww/weapon/36_shotbow/","character/model/1_pc/22_pkww/weapon/47_glaive/",""},
/*STRIKER*/      {"character/model/1_pc/16_pcm/weapon/65_gauntlet/","character/model/1_pc/16_pcm/weapon/66_armlet/","",""}
    };


     char* starterStockings[] = {"pew_00_lb_0010_dec.dds","pew_00_lb_0010_dm_dec.dds","phw_00_uw_0001_03_dec.dds","phw_00_uw_0001_03_dm_dec.dds", "pvw_00_uw_0001_03_dec.dds","pvw_00_uw_0001_03_dm_dec.dds"};

     char* starterUnderwearTextures[] = {"PEW_99_UB_0001_02.dds","PEW_99_UB_0001_dec.dds","PHW_99_UB_0001_02.dds","PHW_99_UB_0001_dec.dds", "PBW_00_UW_0001_dec.dds","PBW_00_UW_0001.dds","PKW_00_UW_0001.dds","PKW_00_UW_0001_dec.dds","pdw_00_uw_0001_dec.dds"};

    char* nudeTextures[] = {"pew_01_nude_0001.dds","pww_01_nude_0001.dds","pbw_00_nude_0001.dds","pdw_00_nude_0001.dds","phw_01_nude_0001.dds"};
    char* pubicHairFolder[] = {
        "patcher_resources\\texture\\",
        "patcher_resources\\texture\\pubic_hair\\shaved_innie\\",
        "patcher_resources\\texture\\pubic_hair\\full_bush\\",
        "patcher_resources\\texture\\pubic_hair\\full_bush_2\\",
        "patcher_resources\\texture\\pubic_hair\\full_bush_3\\",
        "patcher_resources\\texture\\pubic_hair\\medium_bush\\",
        "patcher_resources\\texture\\pubic_hair\\medium_bush2\\",
        "patcher_resources\\texture\\pubic_hair\\small_bush\\",
        "patcher_resources\\texture\\pubic_hair\\small_bush_2\\",
        "patcher_resources\\texture\\pubic_hair\\thin_landing_strip\\",
        "patcher_resources\\texture\\pubic_hair\\wide_landing_strip\\",
        "patcher_resources\\texture\\pubic_hair\\trimmed\\",
        "patcher_resources\\texture\\pubic_hair\\wider_trimmed\\"
       };
    /**
    typedef enum {ARMOR,UNDERWEAR,LOWERBODY,GLOVES,BOOTS,HELMETS,STOCKINGS,WEAPONS,GENDER,SHOULDER,CLOAK} PatcherOption;
    typedef enum {ALL,STARTER,KEEP,REGULAR,LIFE_SKILL,PANTIES_ONLY,IN} Restriction;
    **/


    char* censorshipTextureFiles[] = {
        "pnw_00_ub_0001_dec.dds",
        "pdw_00_lb_0001_dec.dds",
        "pdw_00_lb_0001_dec_dm.dds",
        "pdw_03_lb_0001.dds",
        "pdw_03_lb_0001_dm.dds",
        "pdw_03_ub_0001.dds",
        "pdw_03_ub_0001_dm.dds",
        "pdw_02_ub_0006.dds",
        "pdw_02_ub_0006_dm.dds",
        "pdw_02_lb_0006.dds",
        "pdw_02_lb_0006_dm.dds",
        "pdw_02_lb_0002_dec.dds",
        "pdw_02_lb_0002_dec_dm.dds",
        "pdw_02_sho_0004.dds",
        "pdw_02_sho_0004_dm.dds",
        "pdw_02_lb_0005.dds",
        "pdw_02_lb_0005_dm.dds",
        "pnw_00_lb_0002_dec.dds",
        "pbw_00_ub_0054.dds",
        "pbw_00_ub_0054_dec.dds",
        "pew_00_lb_0033_dec.dds",
        "pbw_00_lb_0018.dds",
        "pdw_00_cloak_0002_dec.dds"
    };

    char* censorshipPACFiles[] = {
        "",
        "",
    };

    char* panties[] = {
        "pbw_00_lb_0007.pac",
		"pbw_00_lb_0007_dm.pac",
		"pbw_00_lb_0015.pac",
		"pbw_00_lb_0016.pac",
		"pbw_00_lb_0016_dm.pac",
		"pbw_00_lb_0017.pac",
		"pbw_00_lb_0017_dm.pac",
		"pbw_02_lb_0001.pac",
		"pbw_02_lb_0001_dm.pac",
		"pbw_02_lb_0002.pac",
		"pbw_02_lb_0003.pac",
		"pbw_02_lb_0004.pac",
		"pbw_02_lb_0005.pac",
		"pbw_02_lb_0005_dm.pac",
		"pbw_02_lb_0006.pac",
		"pbw_02_lb_0006_dm.pac",
		"pbw_03_lb_0001.pac",
		"pbw_03_lb_0002.pac",
		"pbw_03_lb_0002_dm.pac",
		"pbw_10_lb_0001.pac",
		"pbw_10_lb_0003.pac",
		"pbw_10_lb_0006.pac",
		"pbw_10_lb_0017.pac",
		"pew_00_lb_0006.pac",
        "pew_00_lb_0007.pac",
		"pew_01_lb_0001.pac",
		"pew_01_lb_0001_dm.pac",
		"pew_02_lb_0001.pac",
		"pew_02_lb_0001_dm.pac",
		"pew_02_lb_0002.pac",
		"pew_02_lb_0002_dm.pac",
		"pew_02_lb_0003.pac",
		"pew_02_lb_0003_dm.pac",
		"pew_02_lb_0004.pac",
		"pew_02_lb_0004_dm.pac",
		"pew_03_lb_0002.pac",
		"pew_10_lb_0001.pac",
		"pew_10_lb_0003.pac",
		"pew_10_lb_0006.pac",
		"phw_00_lb_0002.pac",
        "phw_00_lb_0005.pac",
		"phw_00_lb_0030.pac",
		"phw_10_lb_0001.pac",
		"phw_10_lb_0003.pac",
		"phw_10_lb_0006.pac"
        "pkww_10_lb_0001.pac",
		"pkww_10_lb_0003.pac",
		"pkww_bw_lb_0015.pac",
        "pnw_02_lb_0001.pac",
		"pnw_10_lb_0001.pac",
		"pnw_10_lb_0003.pac",
		"pnw_bw_lb_0015.pac",
        "pvw_00_lb_0007.pac",
		"pvw_00_lb_0007_01.pac",
		"pvw_00_lb_0007_01_dm.pac",
		"pvw_00_lb_0007_dm.pac",
		"pvw_00_lb_0058.pac",
		"pvw_02_lb_0002.pac",
		"pvw_02_lb_0003.pac",
		"pvw_03_lb_0001.pac",
		"pvw_10_lb_0001.pac",
		"pvw_10_lb_0003.pac",
        "pww_00_lb_0007.pac",
		"pww_00_lb_0007_01.pac",
		"pww_00_lb_0007_01_dm.pac",
		"pww_00_lb_0007_dm.pac",
		"pww_10_lb_0001.pac",
		"pww_10_lb_0003.pac"
		"pew_10_sho_0024.pac",
    };


    char* pants[] = {
        "pew_03_lb_0001.pac",
        "pew_03_lb_0003.pac",
        "phw_00_lb_0007.pac",
        "phw_01_lb_0001.pac",
        "phw_00_lb_0010.pac",
        "phw_00_lb_0049.pac",
        "phw_00_lb_0049_dm.pac",
        "phw_02_lb_0001.pac",
        "phw_02_lb_0002.pac",
        "phw_02_lb_0003.pac",
        "phw_02_lb_0004.pac",
        "phw_03_lb_0001.pac",
        "phw_03_lb_0002.pac",
        "phw_03_lb_0003.pac",
        "phw_20_lb_0003.pac",
        "phw_20_lb_0004.pac",
        "pkww_00_lb_0012.pac",
        "pkww_02_lb_0001.pac",
        "pkww_02_lb_0002.pac",
        "pkww_02_lb_0003.pac",
        "pkww_02_lb_0004.pac",
        "pkww_02_lb_0005.pac",
        "pkww_03_lb_0003.pac",
        "pnw_02_lb_0002.pac",
        "pnw_02_lb_0002_dm.pac",
        "pnw_02_lb_0003.pac",
        "pnw_02_lb_0003_dm.pac",
        "pnw_02_lb_0004.pac",
        "pnw_02_lb_0004_dm.pac",
        "pnw_03_lb_0001.pac",
        "pnw_03_lb_0001_dm.pac",
        "pnw_03_lb_0002.pac",
        "pnw_03_lb_0002_dm.pac",
        "pvw_02_lb_0001.pac",
        "pvw_02_lb_0007.pac",
        "pww_00_lb_0001.pac",
        "pww_02_lb_0001.pac",
        "pww_02_lb_0002.pac",
        "pww_02_lb_0003.pac",
        "pww_03_lb_0001.pac",
        "pww_03_lb_0002.pac",
        "pww_03_lb_0003.pac",
        "pww_03_lb_0004.pac",
    };


    char * stockings[] = {
    "phw_00_uw_0001_03_dec.dds"
    "pew_00_lb_0010_dec.dds"
    "pew_00_uw_0011_02_dec.dds",
    "pew_00_uw_0014_01_dec.dds",
    "pew_00_uw_0030_03_dec.dds",
    "pew_00_uw_0031_dec.dds",
    "pew_00_uw_0035_dec.dds",
    "pew_99_ub_0008_01_dec.dds",
    "phw_00_uw_0002_03_dec.dds",
    "phw_00_uw_0003_03_dec.dds",
    "phw_00_uw_0004_03_dec.dds",
    "phw_00_uw_0005_03_dec.dds",
    "phw_00_uw_0006_03_dec.dds",
    "phw_00_uw_0007_03_dec.dds",
    "phw_00_uw_0011_02_dec.dds",
    "phw_00_uw_0031_dec.dds",
    "phw_00_uw_0032_02_dec.dds",
    "pvw_00_uw_0001_03_dec.dds",
    "pew_00_uw_1034.dds",
    "pew_00_uw_1034_dec.dds"
    };

    char* treedummer_files[] = {
    "pbw_10_ub_0004.pac",
    "pdw_10_lb_0006.pac",
    "pew_10_ub_0004.pac",
    "pgm_10_ub_0004_e.pac",
    "pgm_10_ub_0004_mul_e.pac",
    "pkm_10_ub_0004.pac",
    "pkww_10_lb_0006.pac",
    "pvw_10_lb_0006.pac",
    "pbw_10_ub_0004_dm.pac",
    "pdw_10_lb_0006_dm.pac",
    "pew_10_ub_0004_dm.pac",
    "pgm_10_ub_0004_e_dm.pac",
    "pgm_10_ub_0004_mul_e_dm.pac",
    "pkm_10_ub_0004_dm.pac",
    "pkww_10_lb_0006_dm.pac",
    "pvw_10_lb_0006_dm.pac"
    };


