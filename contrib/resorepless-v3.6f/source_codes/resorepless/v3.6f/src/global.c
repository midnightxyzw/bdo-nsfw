#include "../include/global.h"

// is the variable that contains the 8 bytes that are going to be replaced after each reference byte is found
//int MODDED_BYTES[][2] = {{1,66404},{1,170861},{1,60556},{1,91771}};
//char *MODDED_BYTES[4] = {"\x01\x00\x00\x00\x64\x03\x01\x00","\x01\x00\x00\x00\x6D\x9B\x02\x00","\x01\x00\x00\x00\x8D\xEC\x00\x00","\x01\x00\x00\x00\x7B\x66\x01\x00"};
//                                  NA                                 RU                                   KR                               JP


long filesPatched = 0;

int isWindows10 = 0;

int displayedErrorOnce = 0;

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
/*STRIKER*/      {"character/model/1_pc/16_pcm/weapon/65_gauntlet/","character/model/1_pc/16_pcm/weapon/66_armlet/","character/model/1_pc/16_pcm/weapon/71_godgauntlet/",""},
/*MYSTIC*/       {"character/model/1_pc/16_pcw/weapon/65_gauntlet/","character/model/1_pc/16_pcw/weapon/66_armlet/","character/model/1_pc/16_pcw/weapon/71_godgauntlet/",""},
/*RAN*/       {"character/model/1_pc/17_psw/weapon/69_kusarigama/","character/model/1_pc/17_psw/weapon/70_shotblade/","character/model/1_pc/17_psw/weapon/76_chaintwinsword/",""}
    };


     char* starterStockings[] = {"pew_00_lb_0010_dec.dds","pew_00_lb_0010_dm_dec.dds","phw_00_uw_0001_03_dec.dds","phw_00_uw_0001_03_dm_dec.dds", "pvw_00_uw_0001_03_dec.dds","pvw_00_uw_0001_03_dm_dec.dds"};

     char* starterUnderwearTextures[] = {"PEW_99_UB_0001_02.dds","PEW_99_UB_0001_dec.dds","PHW_99_UB_0001_02.dds","PHW_99_UB_0001_dec.dds", "PBW_00_UW_0001_dec.dds","PBW_00_UW_0001.dds","PKW_00_UW_0001.dds","PKW_00_UW_0001_dec.dds","pdw_00_uw_0001_dec.dds"};

     // enum {RANGER_PUBIC_HAIR,WITCH_PUBIC_HAIR,TAMER_PUBIC_HAIR,DARK_KNIGHT_PUBIC_HAIR,SORCERESS_PUBIC_HAIR,VALKYRIE_PUBIC_HAIR,MAEHWA_PUBIC_HAIR,KUNOICHI_PUBIC_HAIR,MYSTIC_PUBIC_HAIR}
    char* nudeFemaleTextures[] = {
        "pew_01_nude_0001.dds", // RANGER_PUBLIC_HAIR
        "pww_01_nude_0001.dds", // WITCH_PUBLIC_HAIR
        "pbw_00_nude_0001.dds", // TAMER_PUBIC_HAIR
        "pdw_00_nude_0001.dds", // DARK_KNIGHT_PUBIC_HAIR
        "phw_01_nude_0001.dds", // SORCERESS_PUBIC_HAIR
        "pvw_01_nude_0001.dds", // VALKYRIE_PUBIC_HAIR
        "pkw_01_nude_0001.dds", // MAEHWA_PUBIC_HAIR
        "pnw_01_nude_0001.dds", // KUNOICHI_PUBIC_HAIR
        "pcw_01_nude_0001.dds"  // MYSTIC_PUBIC_HAIR
    };
    char* nudeFemaleNormalMaps[] = {"pew_01_nude_0001_n.dds","pbw_00_nude_0001_n.dds","pdw_00_nude_0001_n.dds","phw_01_nude_0001_n.dds"};

    char* nudeFemaleModels[] = {
        "pbw_00_nude_0001.pac",
        "pdw_00_nude_0001.pac",
        "pew_00_nude_0001_noalpha.pac",
        "phw_00_nude_0001_noalpha.pac",
        "pkww_00_nude_0002.pac",
        "pnw_00_nude_0001.pac",
        "pvw_00_nude_0001.pac",
        "pww_00_nude_0001.pac",
        "pcw_00_nude_0001.pac"
    };

     char* sorceressSecundaryNudeTextures[] = {
        "phw_01_nude_0001_m.dds",
        "phw_01_nude_0001_n.dds",
        "phw_01_nude_0001_sp.dds",
        "phw_01_nude_0001_w.dds",
    };

    char* nudeMaleTextures[] = {"phm_00_nude_0001.dds","pgm_00_nude_0001.dds","pkm_00_nude_0001.dds","pwm_00_nude_0001.dds","pwm_01_nude_0001.dds","pcm_00_nude_0001.dds"};

    char* nudeMaleModels[] = {
        "phm_00_nude_0001.pac",
        "pgm_00_nude_0001.pac",
        "pkm_00_nude_0001.pac",
        "pwm_00_nude_0001.pac",
        "pnm_00_nude_0001.pac",
        "pcm_00_nude_0001.pac"
    };


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
        "pdw_00_sho_0002_cull.dds",
        "pdw_00_underup_0002.dds",
        "pdw_00_underup_0002_dec.dds",
        "pdw_02_sho_0004.dds",
        "pdw_02_sho_0004_dm.dds",
        "pdw_02_lb_0005.dds",
        "pdw_02_lb_0005_dm.dds",
        "pnw_00_lb_0002_dec.dds",
        "pbw_00_ub_0054.dds",
        "pbw_00_ub_0054_dec.dds",
        "pew_00_lb_0033_dec.dds",
        "pbw_00_lb_0018.dds",
        "pdw_00_cloak_0002_dec.dds",
    };



    char* panties[] = {
        "pbw_00_lb_0007",
		"pbw_00_lb_0015",
		"pbw_00_lb_0016",
		"pbw_00_lb_0017",
		"pbw_02_lb_0001",
		"pbw_02_lb_0002",
		"pbw_02_lb_0003",
		"pbw_02_lb_0004",
		"pbw_02_lb_0005",
		"pbw_02_lb_0006",
		"pbw_03_lb_0001",
		"pbw_03_lb_0002",
		"pbw_10_lb_0001",
		"pbw_10_lb_0003",
		"pbw_10_lb_0006",
		"pbw_10_lb_0017",
		"pew_00_lb_0006",
        "pew_00_lb_0007",
		"pew_01_lb_0001",
		"pew_02_lb_0001",
		"pew_02_lb_0002",
		"pew_02_lb_0003",
		"pew_02_lb_0004",
		"pew_03_lb_0002",
		"pew_10_lb_0001",
		"pew_10_lb_0003",
		"pew_10_lb_0006",
		"phw_00_lb_0002",
        "phw_00_lb_0005",
		"phw_00_lb_0030",
		"phw_10_lb_0001",
		"phw_10_lb_0003",
		"phw_10_lb_0006"
        "pkww_10_lb_0001",
		"pkww_10_lb_0003",
		"pkww_bw_lb_0015",
        "pnw_02_lb_0001",
		"pnw_10_lb_0001",
		"pnw_10_lb_0003",
		"pnw_bw_lb_0015",
        "pvw_00_lb_0007",
		"pvw_00_lb_0007_01",
		"pvw_00_lb_0058",
		"pvw_02_lb_0002",
		"pvw_02_lb_0003",
		"pvw_03_lb_0001",
		"pvw_10_lb_0001",
		"pvw_10_lb_0003",
        "pww_00_lb_0007",
		"pww_00_lb_0007_01",
		"pww_10_lb_0001",
		"pww_10_lb_0003",
		"pew_10_sho_0024"
    };


    char* pants[] = {
        "pew_03_lb_0001.pac",
        "pew_03_lb_0003.pac",
        "phw_00_lb_0007.pac",
        "phw_01_lb_0001.pac",
        "phw_00_lb_0010.pac",
        "phw_00_lb_0049.pac",
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
        "pnw_02_lb_0003.pac",
        "pnw_02_lb_0004.pac",
        "pnw_03_lb_0001.pac",
        "pnw_03_lb_0002.pac",
        "pvw_02_lb_0001.pac",
        "pvw_02_lb_0007.pac",
        "pww_00_lb_0001.pac",
        "pww_02_lb_0001.pac",
        "pww_02_lb_0002.pac",
        "pww_02_lb_0003.pac",
        "pww_03_lb_0001.pac",
        "pww_03_lb_0002.pac",
        "pww_03_lb_0003.pac",
        "pww_03_lb_0004.pac"
    };


    char * stockings[] = {
    "phw_00_uw_0001_03_dec"
    "pew_00_lb_0010_dec"
    "pew_00_uw_0011_02_dec",
    "pew_00_uw_0014_01_dec",
    "pew_00_uw_0030_03_dec",
    "pew_00_uw_0031_dec",
    "pew_00_uw_0035_dec",
    "pew_99_ub_0008_01_dec",
    "phw_00_uw_0002_03_dec",
    "phw_00_uw_0003_03_dec",
    "phw_00_uw_0004_03_dec",
    "phw_00_uw_0005_03_dec",
    "phw_00_uw_0006_03_dec",
    "phw_00_uw_0007_03_dec",
    "phw_00_uw_0011_02_dec",
    "phw_00_uw_0031_dec",
    "phw_00_uw_0032_02_dec",
    "pvw_00_uw_0001_03_dec",
    "pew_00_uw_1034",
    "pew_00_uw_1034_dec"
    };

    char* treedummer_files[] = {
        "pbw_10_ub_0004",
        "pdw_10_lb_0006",
        "pew_10_ub_0004",
        "pgm_10_ub_0004_e",
        "pgm_10_ub_0004_mul_e",
        "pkm_10_ub_0004",
        "pkww_10_lb_0006",
        "pvw_10_lb_0006"
    };
