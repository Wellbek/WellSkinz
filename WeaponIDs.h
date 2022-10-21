enum CSGO_WEAPON_ID {
	weapon_none,
    weapon_deagle = 1,
    weapon_elite = 2,
    weapon_fiveseven = 3,
    weapon_glock = 4,
    weapon_ak47 = 7,
    weapon_aug = 8,
    weapon_awp = 9,
    weapon_famas = 10,
    weapon_g3sg1 = 11,
    weapon_galilar = 13,
    weapon_m249 = 14,
    weapon_m4a1 = 16,
    weapon_mac10 = 17,
    weapon_p90 = 19,
    weapon_ump = 24,
    weapon_xm1014 = 25,
    weapon_bizon = 26,
    weapon_mag7 = 27,
    weapon_negev = 28,
    weapon_sawedoff = 29,
    weapon_tec9 = 30,
    weapon_hkp2000 = 32,
    weapon_mp7 = 33,
    weapon_mp9 = 34,
    weapon_nova = 35,
    weapon_p250 = 36,
    weapon_scar20 = 38,
    weapon_sg556 = 39,
    weapon_ssg08 = 40,
    weapon_m4a1_silencer = 60,
    weapon_usp_silencer = 61,
    weapon_cz75a = 63,
    weapon_revolver = 64,
};

const char* CSGO_WEAPON_NAMES[] = {
	"weapon_none",
    "weapon_deagle",
    "weapon_elite",
    "weapon_fiveseven",
    "weapon_glock",
	"",
	"",
    "weapon_ak47",
    "weapon_aug",
    "weapon_awp",
    "weapon_famas",
    "weapon_g3sg1",
	"",
    "weapon_galilar",
    "weapon_m249",
	"",
    "weapon_m4a1",
    "weapon_mac10",
	"",
    "weapon_p90",
	"",
	"",
	"",
	"",
    "weapon_ump",
    "weapon_xm1014",
    "weapon_bizon",
    "weapon_mag7",
    "weapon_negev",
    "weapon_sawedoff",
    "weapon_tec9",
	"",
    "weapon_hkp2000",
    "weapon_mp7",
    "weapon_mp9",
    "weapon_nova",
    "weapon_p250",
	"",
    "weapon_scar20",
    "weapon_sg556",
    "weapon_ssg08",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
    "weapon_m4a1_silencer",
    "weapon_usp_silencer",
	"",
    "weapon_cz75a",
    "weapon_revolver",
};

enum CSGO_KNIFE_ID               // id
{
	WEAPON_KNIFE = 42,
	WEAPON_KNIFE_T = 59,
	WEAPON_KNIFE_BAYONET = 500,         // 0
	WEAPON_KNIFE_FLIP = 505,            // 1
	WEAPON_KNIFE_GUT = 506,             // 2
	WEAPON_KNIFE_KARAMBIT = 507,        // 3
	WEAPON_KNIFE_M9_BAYONET = 508,      // 4
	WEAPON_KNIFE_TACTICAL = 509,        // 5
	WEAPON_KNIFE_FALCHION = 512,        // 6
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,  // 7
	WEAPON_KNIFE_BUTTERFLY = 515,       // 8
	WEAPON_KNIFE_PUSH = 516,            // 9
	WEAPON_KNIFE_URSUS = 519,           // 10
	WEAPON_KNIFE_GYPSY_JACKKNIFE = 520, // 11
	WEAPON_KNIFE_STILETTO = 522,        // 12
	WEAPON_KNIFE_WIDOWMAKER = 523       // 13
};

int knifeIndex = 0;