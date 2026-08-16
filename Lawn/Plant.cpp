#include "Coin.h"
#include "Plant.h"
#include "Board.h"
#include "Zombie.h"
#include "Cutscene.h"
#include "GridItem.h"
#include "ZenGarden.h"
#include "Challenge.h"
#include "Projectile.h"
#include "SeedPacket.h"
#include "../LawnApp.h"
#include "CursorObject.h"
#include "../GameConstants.h"
#include "System/PlayerInfo.h"
#include "System/ReanimationLawn.h"
#include "../Sexy.TodLib/TodFoley.h"
#include "../Sexy.TodLib/TodDebug.h"
#include "../Sexy.TodLib/Attachment.h"
#include "../Sexy.TodLib/Reanimator.h"
#include "../Sexy.TodLib/TodParticle.h"
#include "../Sexy.TodLib/EffectSystem.h"
#include "../Sexy.TodLib/TodStringFile.h"
#include <cmath>
#include <vector>

PlantDefinition gPlantDefs[SeedType::NUM_SEED_TYPES] = {  
    // Plant Type,                      Image Pointer, Animation,              Pick Weight,Sun Cost,Seed Recharge,subsclass,                launch rate,  Name
    { SeedType::SEED_PEASHOOTER,        nullptr, ReanimationType::REANIM_PEASHOOTER,    0,  100,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("PEASHOOTER"),       1 },
    { SeedType::SEED_SUNFLOWER,         nullptr, ReanimationType::REANIM_SUNFLOWER,     1,  50,     750,    PlantSubClass::SUBCLASS_NORMAL,     2500,   _S("SUNFLOWER"),        0 },
    { SeedType::SEED_CHERRYBOMB,        nullptr, ReanimationType::REANIM_CHERRYBOMB,    3,  150,    5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("CHERRY_BOMB"),      1 },
    { SeedType::SEED_WALLNUT,           nullptr, ReanimationType::REANIM_WALLNUT,       2,  50,     3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("WALL_NUT"),         0 },
    { SeedType::SEED_POTATOMINE,        nullptr, ReanimationType::REANIM_POTATOMINE,    37, 25,     3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("POTATO_MINE"),      0 },
    { SeedType::SEED_SNOWPEA,           nullptr, ReanimationType::REANIM_SNOWPEA,       4,  175,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("SNOW_PEA"),         2 },
    { SeedType::SEED_CHOMPER,           nullptr, ReanimationType::REANIM_CHOMPER,       31, 150,    750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("CHOMPER"),          0 },
    { SeedType::SEED_REPEATER,          nullptr, ReanimationType::REANIM_REPEATER,      5,  200,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("REPEATER"),         2 },
    { SeedType::SEED_PUFFSHROOM,        nullptr, ReanimationType::REANIM_PUFFSHROOM,    6,  0,      750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("PUFF_SHROOM"),      1 },
    { SeedType::SEED_SUNSHROOM,         nullptr, ReanimationType::REANIM_SUNSHROOM,     7,  25,     750,    PlantSubClass::SUBCLASS_NORMAL,     2000,   _S("SUN_SHROOM"),       0 },
    { SeedType::SEED_FUMESHROOM,        nullptr, ReanimationType::REANIM_FUMESHROOM,    9,  75,     750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("FUME_SHROOM"),      1 },
    { SeedType::SEED_GRAVEBUSTER,       nullptr, ReanimationType::REANIM_GRAVE_BUSTER,  40, 75,     750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("GRAVE_BUSTER"),     0 },
    { SeedType::SEED_HYPNOSHROOM,       nullptr, ReanimationType::REANIM_HYPNOSHROOM,   10, 50,     3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("HYPNO_SHROOM"),     0 },
    { SeedType::SEED_SCAREDYSHROOM,     nullptr, ReanimationType::REANIM_SCRAREYSHROOM, 33, 25,     750,    PlantSubClass::SUBCLASS_SHOOTER,    120,    _S("SCAREDY_SHROOM"),   1 },
    { SeedType::SEED_ICESHROOM,         nullptr, ReanimationType::REANIM_ICESHROOM,     36, 50,     5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("ICE_SHROOM"),       0 },
    { SeedType::SEED_DOOMSHROOM,        nullptr, ReanimationType::REANIM_DOOMSHROOM,    20, 200,    5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("DOOM_SHROOM"),      0 },
    { SeedType::SEED_LILYPAD,           nullptr, ReanimationType::REANIM_LILYPAD,       19, 25,     750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("LILY_PAD"),         0 },
    { SeedType::SEED_SQUASH,            nullptr, ReanimationType::REANIM_SQUASH,        21, 50,     3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SQUASH"),           0 },
    { SeedType::SEED_THREEPEATER,       nullptr, ReanimationType::REANIM_THREEPEATER,   12, 300,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("THREEPEATER"),      1 },
    { SeedType::SEED_TANGLEKELP,        nullptr, ReanimationType::REANIM_TANGLEKELP,    17, 25,     3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("TANGLE_KELP"),      0 },
    { SeedType::SEED_JALAPENO,          nullptr, ReanimationType::REANIM_JALAPENO,      11, 125,    5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("JALAPENO"),         0 },
    { SeedType::SEED_SPIKEWEED,         nullptr, ReanimationType::REANIM_SPIKEWEED,     22, 75,     750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SPIKEWEED"),        0 },
    { SeedType::SEED_TORCHWOOD,         nullptr, ReanimationType::REANIM_TORCHWOOD,     29, 175,    750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("TORCHWOOD"),        0 },
    { SeedType::SEED_TALLNUT,           nullptr, ReanimationType::REANIM_TALLNUT,       28, 100,    3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("TALL_NUT"),         0 },
    { SeedType::SEED_SEASHROOM,         nullptr, ReanimationType::REANIM_SEASHROOM,     39, 0,      1500,   PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("SEA_SHROOM"),       1 },
    { SeedType::SEED_PLANTERN,          nullptr, ReanimationType::REANIM_PLANTERN,      38, 25,     3000,   PlantSubClass::SUBCLASS_NORMAL,     2500,   _S("PLANTERN"),         0 },
    { SeedType::SEED_CACTUS,            nullptr, ReanimationType::REANIM_CACTUS,        15, 125,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("CACTUS"),           2 },
    { SeedType::SEED_BLOVER,            nullptr, ReanimationType::REANIM_BLOVER,        18, 150,    750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("BLOVER"),           0 },
    { SeedType::SEED_SPLITPEA,          nullptr, ReanimationType::REANIM_SPLITPEA,      32, 100,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("SPLIT_PEA"),        1 },
    { SeedType::SEED_STARFRUIT,         nullptr, ReanimationType::REANIM_STARFRUIT,     30, 125,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("STARFRUIT"),        1 },
    { SeedType::SEED_PUMPKINSHELL,      nullptr, ReanimationType::REANIM_PUMPKIN,       25, 100,    1500,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("PUMPKIN"),          0 },
    { SeedType::SEED_MAGNETSHROOM,      nullptr, ReanimationType::REANIM_MAGNETSHROOM,  35, 100,    750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("MAGNET_SHROOM"),    0 },
    { SeedType::SEED_CABBAGEPULT,       nullptr, ReanimationType::REANIM_CABBAGEPULT,   13, 100,    750,    PlantSubClass::SUBCLASS_SHOOTER,    300,    _S("CABBAGE_PULT"),     1 },
    { SeedType::SEED_FLOWERPOT,         nullptr, ReanimationType::REANIM_FLOWER_POT,    33, 25,     750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("FLOWER_POT"),       0 },
    { SeedType::SEED_KERNELPULT,        nullptr, ReanimationType::REANIM_KERNELPULT,    13, 100,    750,    PlantSubClass::SUBCLASS_SHOOTER,    300,    _S("KERNEL_PULT"),      1 },
    { SeedType::SEED_INSTANT_COFFEE,    nullptr, ReanimationType::REANIM_COFFEEBEAN,    33, 50,     1500,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("COFFEE_BEAN"),      0 },
    { SeedType::SEED_GARLIC,            nullptr, ReanimationType::REANIM_GARLIC,        8,  50,     750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("GARLIC"),           0 },
    { SeedType::SEED_UMBRELLA,          nullptr, ReanimationType::REANIM_UMBRELLALEAF,  23, 100,    750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("UMBRELLA_LEAF"),    0 },
    { SeedType::SEED_MARIGOLD,          nullptr, ReanimationType::REANIM_MARIGOLD,      24, 0,      5000,   PlantSubClass::SUBCLASS_NORMAL,     2500,   _S("MARIGOLD"),         0 },
    { SeedType::SEED_MELONPULT,         nullptr, ReanimationType::REANIM_MELONPULT,     14, 300,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("MELON_PULT"),       1 },
    { SeedType::SEED_SUNBEAN,           nullptr, ReanimationType::REANIM_SUNBEAN,       0,  50,     3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SUN_BEAN"),         0 },
    { SeedType::SEED_COMMANDOPEA,       nullptr, ReanimationType::REANIM_COMMANDOPEA,   0,  350,    3000,   PlantSubClass::SUBCLASS_SHOOTER,    900,    _S("COMMANDO_PEA"),     1 },
    { SeedType::SEED_SWEETPOTATO,       nullptr, ReanimationType::REANIM_SWEETPOTATO,   0,  75,     3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SWEET_POTATO"),     0 },
    { SeedType::SEED_SHRINKING_VIOLET,  nullptr, ReanimationType::REANIM_SHRINKING_VIOLET, 37, 50,  3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SHRINKING_VIOLET"), 0 },
    { SeedType::SEED_SPROUT,            nullptr, ReanimationType::REANIM_ZENGARDEN_SPROUT,33, 200,  3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SPROUT"),           0 },
    { SeedType::SEED_HATTREMWITCH,      nullptr, ReanimationType::REANIM_HATTREMWITCH,  0,  250,    3000,   PlantSubClass::SUBCLASS_SHOOTER,    1000,   _S("HATTREM_WITCH"),    0 },
    { SeedType::SEED_PICKLEDPEPPER,     nullptr, ReanimationType::REANIM_PICKLEDPEPPER, 0,  125,    3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("PICKLED_PEPPER"),   0 },
    { SeedType::SEED_NIGHTCAP,          nullptr, ReanimationType::REANIM_NIGHTCAP,      0,  150,    1500,   PlantSubClass::SUBCLASS_SHOOTER,    25,     _S("NIGHTCAP"),         0 },
    { SeedType::SEED_IMITATER,          nullptr, ReanimationType::REANIM_IMITATER,      33, 0,      750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("IMITATER"),         0 },

    { SeedType::SEED_GATLINGPEA,        nullptr, ReanimationType::REANIM_GATLINGPEA,    5,  250,    5000,   PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("GATLING_PEA"),      1 },
    { SeedType::SEED_TWINSUNFLOWER,     nullptr, ReanimationType::REANIM_TWIN_SUNFLOWER,1,  200,    2000,   PlantSubClass::SUBCLASS_NORMAL,     2500,   _S("TWIN_SUNFLOWER"),   0 },
    { SeedType::SEED_GLOOMSHROOM,       nullptr, ReanimationType::REANIM_GLOOMSHROOM,   27, 150,    2000,   PlantSubClass::SUBCLASS_SHOOTER,    200,    _S("GLOOM_SHROOM"),     1 },
    { SeedType::SEED_CATTAIL,           nullptr, ReanimationType::REANIM_CATTAIL,       27, 255,    3000,   PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("CATTAIL"),          1 },
    { SeedType::SEED_WINTERMELON,       nullptr, ReanimationType::REANIM_WINTER_MELON,  27, 500,    5000,   PlantSubClass::SUBCLASS_SHOOTER,    300,    _S("WINTER_MELON"),     1 },
    { SeedType::SEED_GOLD_MAGNET,       nullptr, ReanimationType::REANIM_GOLD_MAGNET,   27, 100,    750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("GOLD_MAGNET"),      0 },
    { SeedType::SEED_SPIKEROCK,         nullptr, ReanimationType::REANIM_SPIKEROCK,     27, 225,    3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SPIKEROCK"),        0 },
    { SeedType::SEED_COBCANNON,         nullptr, ReanimationType::REANIM_COBCANNON,     16, 700,    5000,   PlantSubClass::SUBCLASS_NORMAL,     600,    _S("COB_CANNON"),       1 },
    { SeedType::SEED_GIANT_WALLNUT,     nullptr, ReanimationType::REANIM_WALLNUT,       2,  0,      3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("GIANT_WALLNUT"),    0 },
    { SeedType::SEED_LEFTPEATER,        nullptr, ReanimationType::REANIM_REPEATER,      5,  25,     750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("REPEATER"),         1 },
    // new plant goes here
    { SeedType::SEED_FIRESHOOTER,       nullptr, ReanimationType::REANIM_FIRESHOOTER,   0,  175,    1500,   PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("FIRESHOOTER"),      1 },
    { SeedType::SEED_EXPLODE_O_NUT,     nullptr, ReanimationType::REANIM_EXPLODE_O_NUT, 0,  150,    5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("EXPLODE_O_NUT"),    0 },
    { SeedType::SEED_RED_POTATO_MINE,   nullptr, ReanimationType::REANIM_RED_POTATO_MINE,   37, 75,   3000, PlantSubClass::SUBCLASS_NORMAL,     0,      _S("RED_POTATO_MINE"),  0 },
    { SeedType::SEED_ICEPEA,            nullptr, ReanimationType::REANIM_ICEPEA,        0,  250,    3000,   PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("ICE_PEA"),          1 },
    { SeedType::SEED_SUPER_CHOMPER,     nullptr, ReanimationType::REANIM_SUPER_CHOMPER, 0,  300,    3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SUPER_CHOMPER"),    0 },
    { SeedType::SEED_HYPNOGIGA,         nullptr, ReanimationType::REANIM_HYPNOGIGA,     0,  150,    3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("HYPNOGIGA"),        0 },
    { SeedType::SEED_BOMBSQUASH,        nullptr, ReanimationType::REANIM_BOMBSQUASH,    0,  150,    3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("BOMBSQUASH"),       0 },
    { SeedType::SEED_BLACK_THREEPEATER, nullptr, ReanimationType::REANIM_BLACK_THREEPEATER, 0, 500, 3000,   PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("BLACK_THREEPEATER"),1 },
    { SeedType::SEED_BLUE_TORCHWOOD,    nullptr, ReanimationType::REANIM_BLUE_TORCHWOOD,0,  350,    3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("BLUE_TORCHWOOD"),   0 },
    { SeedType::SEED_DOOM_NUT,          nullptr, ReanimationType::REANIM_DOOM_NUT,      0,  250,    5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("DOOM_NUT"),         0 },
    { SeedType::SEED_ICE_PLANTERN,      nullptr, ReanimationType::REANIM_ICE_PLANTERN,  0,  125,    5000,   PlantSubClass::SUBCLASS_NORMAL,     2500,   _S("ICE_PLANTERN"),     0 },
    { SeedType::SEED_MAD_CACTUS,        nullptr, ReanimationType::REANIM_MAD_CACTUS,    0,  325,    3000,   PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("CACTUS"),           3 },
    { SeedType::SEED_SOUR_STARFRUIT,    nullptr, ReanimationType::REANIM_SOUR_STARFRUIT,0,  375,    3000,   PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("SOUR_STARFRUIT"),   1 },
    { SeedType::SEED_POISONPULT,        nullptr, ReanimationType::REANIM_POISONPULT,    0,  300,    5000,   PlantSubClass::SUBCLASS_SHOOTER,    400,    _S("POISON_PULT"),      1 },
    { SeedType::SEED_CORROSION_GARLIC,  nullptr, ReanimationType::REANIM_CORROSION_GARLIC,0,  100,  3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("CORROSION_GARLIC"), 0 },
    { SeedType::SEED_ABSOLUTELEAF,      nullptr, ReanimationType::REANIM_ABSOLUTELEAF,  0,  300,    5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("ABSOLUTE_LEAF"),    0 },
    { SeedType::SEED_SUNBEAN_BOMB,      nullptr, ReanimationType::REANIM_SUNBEAN_BOMB,  0,  100,    5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SUN_BEAN"),         0 },
    { SeedType::SEED_GENERALPEA,        nullptr, ReanimationType::REANIM_GENERALPEA,    0,  700,    5000,   PlantSubClass::SUBCLASS_SHOOTER,    750,    _S("GENERAL_PEA"),      1 },
    { SeedType::SEED_SWEETEST_POTATO,   nullptr, ReanimationType::REANIM_SWEETEST_POTATO,0, 150,    5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SWEETEST_POTATO"),  0 },
    { SeedType::SEED_HATTREMSAGE,       nullptr, ReanimationType::REANIM_HATTREMSAGE,   0,  500,    5000,   PlantSubClass::SUBCLASS_SHOOTER,    600,    _S("HATTREM_WITCH"),    0 },
    { SeedType::SEED_DARKCAP,           nullptr, ReanimationType::REANIM_DARKCAP,       0,  400,    5000,   PlantSubClass::SUBCLASS_SHOOTER,    25,     _S("NIGHTCAP"),         0 },
    { SeedType::SEED_SUPER_SPLITPEA,    nullptr, ReanimationType::REANIM_SPLITPEA,      32, 225,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("SUPER_SPLITPEA"),   1 },
};

Plant::Plant()
{
}

void Plant::PlantInitialize(int theGridX, int theGridY, SeedType theSeedType, SeedType theImitaterType)
{
    mPlantCol = theGridX;
    mRow = theGridY;
    if (mBoard)
    {
        mX = mBoard->GridToPixelX(theGridX, theGridY);
        mY = mBoard->GridToPixelY(theGridX, theGridY);
    } 
    mAnimCounter = 0;
    mAnimPing = true;
    mFrame = 0;
    mShootingCounter = 0;
    mShakeOffsetX = 0.0f;
    mShakeOffsetY = 0.0f;
    mFrameLength = RandRangeInt(12, 18);
    mTargetX = -1;
    mTargetY = -1;
    mStartRow = mRow;
    mNumFrames = 5;
    mState = PlantState::STATE_NOTREADY;
    mDead = false;
    mSquished = false;
    mSeedType = theSeedType;
    mImitaterType = theImitaterType;
    mPlantHealth = 300;
    mDoSpecialCountdown = 0;
    mDisappearCountdown = 200;
    mStateCountdown = 0;
    mSpecialAttackCount = 0;
    mParticleID = ParticleSystemID::PARTICLESYSTEMID_NULL;
    mBodyReanimID = ReanimationID::REANIMATIONID_NULL;
    mHeadReanimID = ReanimationID::REANIMATIONID_NULL;
    mHeadReanimID2 = ReanimationID::REANIMATIONID_NULL;
    mHeadReanimID3 = ReanimationID::REANIMATIONID_NULL;
    mBlinkReanimID = ReanimationID::REANIMATIONID_NULL;
    mLightReanimID = ReanimationID::REANIMATIONID_NULL;
    mSleepingReanimID = ReanimationID::REANIMATIONID_NULL;
    mBlinkCountdown = 0;
    mRecentlyEatenCountdown = 0;
    mEatenFlashCountdown = 0;
    mBeghouledFlashCountdown = 0;
    mWidth = 80;
    mHeight = 80;
    memset(mMagnetItems, 0, sizeof(mMagnetItems));
    const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
    mIsAsleep = false;
    mWakeUpCounter = 0;
    mOnBungeeState = PlantOnBungeeState::NOT_ON_BUNGEE;
    mPottedPlantIndex = -1;
    mLaunchRate = aPlantDef.mLaunchRate;
    mSubclass = aPlantDef.mSubClass;
    mRenderOrder = CalcRenderOrder();
    mShootingBurstCount = 0;
    mShootingBurstTimer = 0;
    mHealCountdown = 0;
    mPlantMaxHealthOriginal = 0;
    mIsBoostedByPlantern = false;
    mIsOnPotAndLily = false;
    mIsBoosted = false;
    mBoostCounter = 0;
    mWasShoveled = false;
    mIcePlanternChillCounter = 0;
    mIsIceBoosted = false;
    mSourStarfruitShotCount = 0;
    mChilledCounter = 0;
    mFrozenCounter = 0;
    mButteredCounter = 0;
    mButterX = 0;
    mButterY = 0;
    mSunshroomSteaks = 0;
    mGraveBusterGraveTarget = nullptr;
    mGraveBusterPlantTarget = nullptr;
    mEatingZombieID = ZOMBIEID_NULL;
    mIsBlowing = false;
    mShieldHealth = 0;
    mShieldMaxHealth = 0;
    if (mSeedType == SeedType::SEED_PLANTERN || mImitaterType == SeedType::SEED_PLANTERN)
    {
        mShieldMaxHealth = 2000;
        mShieldHealth = 2000;
        GrantPlacementShieldPulse();
    }
    else if (mSeedType == SeedType::SEED_ICE_PLANTERN || mImitaterType == SeedType::SEED_ICE_PLANTERN)
    {
        mShieldMaxHealth = 4000;
        mShieldHealth = 4000;
        GrantPlacementShieldPulse();
    }
    else if (mBoard && mSeedType != SeedType::SEED_PUMPKINSHELL && mSeedType != SeedType::SEED_FLOWERPOT && mImitaterType != SeedType::SEED_FLOWERPOT)
    {
        SeedType aEffectiveSeed = (mSeedType == SeedType::SEED_IMITATER) ? mImitaterType : mSeedType;
        if (aEffectiveSeed != SeedType::SEED_JALAPENO && 
            aEffectiveSeed != SeedType::SEED_CHERRYBOMB && 
            aEffectiveSeed != SeedType::SEED_DOOMSHROOM && 
            aEffectiveSeed != SeedType::SEED_ICESHROOM && 
            aEffectiveSeed != SeedType::SEED_PICKLEDPEPPER &&
            aEffectiveSeed != SeedType::SEED_EXPLODE_O_NUT)
        {
            CheckAndReceiveNearbyPlanternShield();
        }
    }

    Reanimation* aBodyReanim = nullptr;
    if (aPlantDef.mReanimationType != ReanimationType::REANIM_NONE)
    {
        float aOffsetY = PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);
        aBodyReanim = mApp->AddReanimation(0.0f, aOffsetY, mRenderOrder + 1, aPlantDef.mReanimationType);
        aBodyReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
        aBodyReanim->mAnimRate = RandRangeFloat(10.0f, 15.0f);

        if (aBodyReanim->TrackExists("anim_idle"))
            aBodyReanim->SetFramesForLayer("anim_idle");

        if (mApp->IsWallnutBowlingLevel() && aBodyReanim->TrackExists("_ground"))
        {
            aBodyReanim->SetFramesForLayer("_ground");
            if (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_EXPLODE_O_NUT)
                aBodyReanim->mAnimRate = RandRangeFloat(12.0f, 18.0f);
            else if (mSeedType == SeedType::SEED_GIANT_WALLNUT)
                aBodyReanim->mAnimRate = RandRangeFloat(6.0f, 10.0f);
        }

        aBodyReanim->mIsAttachment = true;
        mBodyReanimID = mApp->ReanimationGetID(aBodyReanim);
        mBlinkCountdown = 400 + Sexy::Rand(400);
    }

    if (IsNocturnal(mSeedType) && mBoard && !mBoard->StageIsNight())
        SetSleeping(true);

    if (mLaunchRate > 0)
    {
        if (MakesSun())
            mLaunchCounter = RandRangeInt(300, mLaunchRate / 2);
        else
            mLaunchCounter = RandRangeInt(0, mLaunchRate);
    }
    else
        mLaunchCounter = 0;

    switch (theSeedType)
    {
    case SeedType::SEED_BLOVER:
    {
        mDoSpecialCountdown = 50;

        if (IsInPlay())
        {
            aBodyReanim->SetFramesForLayer("anim_blow");
            aBodyReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
            aBodyReanim->mAnimRate = 20.0f;
        }
        else
        {
            aBodyReanim->SetFramesForLayer("anim_idle");
            aBodyReanim->mAnimRate = 10.0f;
        }

        break;
    }
    case SeedType::SEED_PEASHOOTER:
    case SeedType::SEED_SNOWPEA:
    case SeedType::SEED_REPEATER:
    case SeedType::SEED_LEFTPEATER:
    case SeedType::SEED_GATLINGPEA:
    case SeedType::SEED_FIRESHOOTER:
    case SeedType::SEED_ICEPEA:
        if (aBodyReanim)
        {
            aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
            Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim->SetFramesForLayer("anim_head_idle");
            mHeadReanimID = mApp->ReanimationGetID(aHeadReanim);

            if (aBodyReanim->TrackExists("anim_stem"))
                aHeadReanim->AttachToAnotherReanimation(aBodyReanim, "anim_stem");
            else if (aBodyReanim->TrackExists("anim_idle"))
                aHeadReanim->AttachToAnotherReanimation(aBodyReanim, "anim_idle");
        }
        break;
    case SeedType::SEED_SPLITPEA:
    case SeedType::SEED_SUPER_SPLITPEA:
    {
        TOD_ASSERT(aBodyReanim);

        aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
        Reanimation* aHeadReanim1 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim1->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim1->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim1->SetFramesForLayer("anim_head_idle");
        aHeadReanim1->AttachToAnotherReanimation(aBodyReanim, "anim_idle");
        mHeadReanimID = mApp->ReanimationGetID(aHeadReanim1);

        Reanimation* aHeadReanim2 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim2->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim2->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim2->SetFramesForLayer("anim_splitpea_idle");
        aHeadReanim2->AttachToAnotherReanimation(aBodyReanim, "anim_idle");
        mHeadReanimID2 = mApp->ReanimationGetID(aHeadReanim2);

        if (mSeedType == SeedType::SEED_SUPER_SPLITPEA)
        {
            Color aColor(255, 140, 255);
            aBodyReanim->mColorOverride = aColor;
            aHeadReanim1->mColorOverride = aColor;
            aHeadReanim2->mColorOverride = aColor;
        }

        break;
    }
    case SeedType::SEED_THREEPEATER:
    {
        TOD_ASSERT(aBodyReanim);

        aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
        Reanimation* aHeadReanim1 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim1->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim1->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim1->SetFramesForLayer("anim_head_idle1");
        aHeadReanim1->AttachToAnotherReanimation(aBodyReanim, "anim_head1");
        mHeadReanimID = mApp->ReanimationGetID(aHeadReanim1);

        Reanimation* aHeadReanim2 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim2->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim2->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim2->SetFramesForLayer("anim_head_idle2");
        aHeadReanim2->AttachToAnotherReanimation(aBodyReanim, "anim_head2");
        mHeadReanimID2 = mApp->ReanimationGetID(aHeadReanim2);

        Reanimation* aHeadReanim3 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim3->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim3->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim3->SetFramesForLayer("anim_head_idle3");
        aHeadReanim3->AttachToAnotherReanimation(aBodyReanim, "anim_head3");
        mHeadReanimID3 = mApp->ReanimationGetID(aHeadReanim3);

        break;
    }
    case SeedType::SEED_BLACK_THREEPEATER:
    {
        TOD_ASSERT(aBodyReanim);
        mBlackThreepeaterShotCount = 0; // Initialize the counter

        aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
        Reanimation* aHeadReanim1 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim1->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim1->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim1->SetFramesForLayer("anim_head_idle1");
        aHeadReanim1->AttachToAnotherReanimation(aBodyReanim, "anim_head1");
        mHeadReanimID = mApp->ReanimationGetID(aHeadReanim1);

        Reanimation* aHeadReanim2 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim2->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim2->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim2->SetFramesForLayer("anim_head_idle2");
        aHeadReanim2->AttachToAnotherReanimation(aBodyReanim, "anim_head2");
        mHeadReanimID2 = mApp->ReanimationGetID(aHeadReanim2);

        Reanimation* aHeadReanim3 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim3->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim3->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim3->SetFramesForLayer("anim_head_idle3");
        aHeadReanim3->AttachToAnotherReanimation(aBodyReanim, "anim_head3");
        mHeadReanimID3 = mApp->ReanimationGetID(aHeadReanim3);

        break;
    }
    case SeedType::SEED_WALLNUT:
    case SeedType::SEED_EXPLODE_O_NUT:
        mPlantHealth = 4000;
        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        break;
    case SeedType::SEED_GIANT_WALLNUT:
        mPlantHealth = 12000;
        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        break;
    case SeedType::SEED_TALLNUT:
        mPlantHealth = 8000;
        mHeight = 80;
        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        break;
    case SeedType::SEED_DOOM_NUT:
        mPlantHealth = 8000;
        mState = PlantState::STATE_NOTREADY;
        mHeight = 80;
        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        break;
    case SeedType::SEED_GARLIC:
        TOD_ASSERT(aBodyReanim);
        mPlantHealth = 2000;
        aBodyReanim->SetTruncateDisappearingFrames();
        break;
    case SeedType::SEED_UMBRELLA:
        mPlantHealth = 2000;
        break;
    case SeedType::SEED_ABSOLUTELEAF:
        mPlantHealth = 5000;
        mPushCounter = 0;
        mPushCooldown = 0;
        // Imp push logic
        if (IsOnBoard())
        {
            Rect aPushArea(mX - 80, mY - 85, 240, 255); // 3x3 tile area
            Zombie* aZombie = nullptr;
            while (mBoard->IterateZombies(aZombie))
            {
                if (aZombie->mZombieType == ZOMBIE_IMP && !aZombie->IsDeadOrDying())
                {
                    Rect aZombieRect = aZombie->GetZombieRect();
                    if (aPushArea.Intersects(aZombieRect))
                    {
                        // Push the imp to the last tile (column 8)
                        aZombie->mPosX = mBoard->GridToPixelX(8, aZombie->mRow);
                    }
                }
            }
            mApp->PlayFoley(FOLEY_BONK);
        }
        break;
    case SeedType::SEED_CORROSION_GARLIC:
        mPlantHealth = 4000;
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->SetTruncateDisappearingFrames();
        break;
    case SeedType::SEED_GOLD_MAGNET:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->SetTruncateDisappearingFrames();
        mState = PlantState::STATE_READY;
        break;
    case SeedType::SEED_IMITATER:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->mAnimRate = RandRangeFloat(25.0f, 30.0f);
        mStateCountdown = 200;
        break;
    case SeedType::SEED_CHERRYBOMB:
    case SeedType::SEED_JALAPENO:
    case SeedType::SEED_PICKLEDPEPPER:
    {
        TOD_ASSERT(aBodyReanim);

        if (IsInPlay())
        {
            mDoSpecialCountdown = 100;

            aBodyReanim->SetFramesForLayer("anim_explode");
            aBodyReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;

            mApp->PlayFoley(FoleyType::FOLEY_REVERSE_EXPLOSION);
        }

        break;
    }
    case SeedType::SEED_POTATOMINE:
    {
        mPlantHealth = 1000;
        TOD_ASSERT(aBodyReanim);

        aBodyReanim->mAnimRate = 12.0f;

        if (IsInPlay())
        {
            aBodyReanim->AssignRenderGroupToTrack("anim_glow", RENDER_GROUP_HIDDEN);
            mStateCountdown = 1500;
        }
        else
        {
            aBodyReanim->SetFramesForLayer("anim_armed");
            mState = PlantState::STATE_POTATO_ARMED;
        }

        break;
    }
    case SEED_RED_POTATO_MINE:
        TOD_ASSERT(aBodyReanim);

        aBodyReanim->mAnimRate = 12.0f;

        if (IsInPlay())
        {
            aBodyReanim->AssignRenderGroupToTrack("anim_glow", RENDER_GROUP_HIDDEN);
            mStateCountdown = 5;
        }
        else
        {
            aBodyReanim->SetFramesForLayer("anim_armed");
            mState = PlantState::STATE_POTATO_ARMED;
        }
        mIsAsleep = false;
        break;
    case SeedType::SEED_GRAVEBUSTER:
    {
        TOD_ASSERT(aBodyReanim);

        if (IsInPlay())
        {
            aBodyReanim->SetFramesForLayer("anim_land");
            aBodyReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;

            mState = PlantState::STATE_GRAVEBUSTER_LANDING;
            mApp->PlayFoley(FoleyType::FOLEY_GRAVEBUSTERCHOMP);
        }
        mPlantHealth = 10000;

        if (mBoard && mApp->mGameScene == GameScenes::SCENE_PLAYING)
        {
            mGraveBusterGraveTarget = mBoard->GetGraveStoneAt(mPlantCol, mRow);
            if (mGraveBusterGraveTarget == nullptr)
            {
                // Find the plant that was here previously (if any)
                Plant* aPlant = nullptr;
                while (mBoard->IteratePlants(aPlant))
                {
                    if (aPlant != this && aPlant->mPlantCol == mPlantCol &&
                        aPlant->mRow == mRow && !aPlant->NotOnGround())
                    {
                        if (aPlant->mSeedType != SEED_PUMPKINSHELL)
                        {
                            mGraveBusterPlantTarget = aPlant;
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            // In Almanac or non-playing modes, disable gravebuster behavior.
            mGraveBusterGraveTarget = nullptr;
        }
        break;
    }
    case SeedType::SEED_SUNSHROOM:
    {
        TOD_ASSERT(aBodyReanim);

        aBodyReanim->mFrameBasePose = 6;

        if (IsInPlay())
        {
            mX += Sexy::Rand(10) - 5;
            mY += Sexy::Rand(10) - 5;
        }
        else if (mIsAsleep)
            aBodyReanim->SetFramesForLayer("anim_bigsleep");
        else
            aBodyReanim->SetFramesForLayer("anim_bigidle");

        mState = PlantState::STATE_SUNSHROOM_SMALL;
        mStateCountdown = 12000;

        break;
    }
    case SeedType::SEED_PUFFSHROOM:
    case SeedType::SEED_SEASHROOM:
        if (IsInPlay())
        {
            mX += Sexy::Rand(10) - 5;
            mY += Sexy::Rand(6) - 3;
        }
        break;
    case SeedType::SEED_GLOOMSHROOM:
        mGloomMode = GLOOM_NORMAL;
        mGloomShotCount = 0;
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->SetTruncateDisappearingFrames();
        break;
    case SeedType::SEED_SQUASH:
    case SeedType::SEED_BOMBSQUASH:
        mState = PlantState::STATE_NOTREADY;
        mSquashJumpCount = 0;
        break;
    case SeedType::SEED_PUMPKINSHELL:
    {
        mPlantHealth = 4000;
        mWidth = 120;

        TOD_ASSERT(aBodyReanim);
        aBodyReanim->AssignRenderGroupToTrack("Pumpkin_back", 1);
        break;
    }
    case SeedType::SEED_CHOMPER:
        mState = PlantState::STATE_READY;
        mPlantHealth = 2000;
        break;
    case SeedType::SEED_SUPER_CHOMPER:
        mPlantHealth = 4000;
        mState = PlantState::STATE_READY;
        break;
    case SeedType::SEED_PLANTERN:
    case SeedType::SEED_ICE_PLANTERN:
    {
        mStateCountdown = 50;
        if (IsInPlay())
        {
            mApp->PlaySample(Sexy::SOUND_PLANTERN);
        }
        
        break;
    }
    case SeedType::SEED_BLUE_TORCHWOOD:
    case SeedType::SEED_TORCHWOOD:
        mPlantHealth = 1500;
        break;
    case SeedType::SEED_MARIGOLD:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
        if (IsInPlay())
        {
            mState = STATE_MARIGOLD_JUMPING;
            mStateCountdown = 50; // Time for the first jump
            mJumpCounter = 0;
        }
        break;
    case SeedType::SEED_CACTUS:
    case SeedType::SEED_MAD_CACTUS:
        mState = PlantState::STATE_CACTUS_LOW;
        break;
    case SeedType::SEED_INSTANT_COFFEE:
        mDoSpecialCountdown = 100;
        break;
    case SeedType::SEED_SCAREDYSHROOM:
        mState = PlantState::STATE_READY;
        if (mPlantCol >= 6 && !mIsAsleep)
        {
            mPlantHealth = 1000;
            mPlantMaxHealth = 1000;
        }
        break;
    case SeedType::SEED_COBCANNON:
        if (IsInPlay())
        {
            mState = PlantState::STATE_COBCANNON_ARMING;
            mStateCountdown = 500;

            TOD_ASSERT(aBodyReanim);
            aBodyReanim->SetFramesForLayer("anim_unarmed_idle");
        }
        break;
    case SeedType::SEED_KERNELPULT:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->AssignRenderGroupToPrefix("Cornpult_butter", RENDER_GROUP_HIDDEN);
        break;
    case SeedType::SEED_MAGNETSHROOM:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->SetTruncateDisappearingFrames();
        mState = PlantState::STATE_READY;
        break;
    case SeedType::SEED_SPIKEROCK:
        mPlantHealth = 750;
        TOD_ASSERT(aBodyReanim);
        break;
    case SeedType::SEED_SPROUT:
        mState = STATE_SPROUT_TRANSFORMING;
        mStateCountdown = 200;
        break;
    case SeedType::SEED_COMMANDOPEA:
    case SeedType::SEED_GENERALPEA:
        if (aBodyReanim)
        {
            aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
            Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim->SetFramesForLayer("anim_head_idle");
            mHeadReanimID = mApp->ReanimationGetID(aHeadReanim);

            if (aBodyReanim->TrackExists("anim_stem"))
                aHeadReanim->AttachToAnotherReanimation(aBodyReanim, "anim_stem");
            else if (aBodyReanim->TrackExists("anim_idle"))
                aHeadReanim->AttachToAnotherReanimation(aBodyReanim, "anim_idle");
        }
        mState = STATE_READY;
        break;
    case SeedType::SEED_SHRINKING_VIOLET:
        if (aBodyReanim)
        {
            aBodyReanim->SetFramesForLayer("anim_idle");
            int aFrameStart = aBodyReanim->mFrameStart;
            int aFrameCount = aBodyReanim->mFrameCount;

            for (int i = 0; i < aBodyReanim->mDefinition->mTrackCount; i++)
            {
                ReanimatorTrack* aTrack = &aBodyReanim->mDefinition->mTracks[i];
                bool hasFrameInIdle = false;
                for (int j = aFrameStart; j < aFrameStart + aFrameCount && j < aTrack->mTransformCount; j++)
                {
                    if (aTrack->mTransforms[j].mFrame >= 0.0f)
                    {
                        hasFrameInIdle = true;
                        break;
                    }
                }
                if (!hasFrameInIdle)
                {
                    aBodyReanim->mTrackInstances[i].mRenderGroup = RENDER_GROUP_HIDDEN;
                }
            }
        }
        mState = STATE_NOTREADY;
        mStateCountdown = 50;
        break;
        
    case SeedType::SEED_HATTREMWITCH:
    case SeedType::SEED_HATTREMSAGE:
        if (aBodyReanim)
        {
            aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
            Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim->SetFramesForLayer("anim_head_idle");
            mHeadReanimID = mApp->ReanimationGetID(aHeadReanim);
            if (aBodyReanim->TrackExists("anim_stem"))
            {
                aHeadReanim->AttachToAnotherReanimation(aBodyReanim, "anim_stem");
            }
            else if (aBodyReanim->TrackExists("anim_idle"))
            {
                aHeadReanim->AttachToAnotherReanimation(aBodyReanim, "anim_idle");
            }
        }
        mState = STATE_READY;
        break;
    case SeedType::SEED_NIGHTCAP:
    case SeedType::SEED_DARKCAP:
        mState = STATE_READY;
        mIsHiding = false;
        break;
    case SeedType::SEED_SWEETPOTATO:
    case SeedType::SEED_SWEETEST_POTATO:
        if (aBodyReanim)
        {
            mPlantHealth = 4000;
            Reanimation* aFaceReanim = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 1, aPlantDef.mReanimationType);
            aFaceReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aFaceReanim->SetFramesForLayer("anim_face");
            aFaceReanim->mAnimRate = aBodyReanim->mAnimRate;
            aFaceReanim->AttachToAnotherReanimation(aBodyReanim, "anim_idle");
        }
        break;
    case SeedType::SEED_SUNBEAN:
        if (aBodyReanim)
        {
            Reanimation* aFaceReanim = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 1, aPlantDef.mReanimationType);
            aFaceReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aFaceReanim->mAnimRate = aBodyReanim->mAnimRate;
            aFaceReanim->SetFramesForLayer("anim_face");
            aFaceReanim->mAnimRate = aBodyReanim->mAnimRate;
            aFaceReanim->AttachToAnotherReanimation(aBodyReanim, "anim_idle");
        }
        break;
    case SeedType::SEED_SUNBEAN_BOMB:
        if (aBodyReanim)
        {
            mPlantHealth = 1000;
            Reanimation* aFaceReanim = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 1, aPlantDef.mReanimationType);
            aFaceReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aFaceReanim->mAnimRate = aBodyReanim->mAnimRate;
            aFaceReanim->SetFramesForLayer("anim_face");
            aFaceReanim->mAnimRate = aBodyReanim->mAnimRate;
            aFaceReanim->AttachToAnotherReanimation(aBodyReanim, "anim_idle");
        }
        break;
    case SeedType::SEED_FLOWERPOT:
        if (IsInPlay())
        {
            mState = PlantState::STATE_FLOWERPOT_INVULNERABLE;
            mStateCountdown = 100;
        }
        break;
    case SeedType::SEED_LILYPAD:
        if (IsInPlay())
        {
            mState = PlantState::STATE_LILYPAD_INVULNERABLE;
            mStateCountdown = 100;
        }
        break;
    case SeedType::SEED_TANGLEKELP:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->SetTruncateDisappearingFrames();
    }
    
    if ((mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME) &&
        (theSeedType == SeedType::SEED_WALLNUT || theSeedType == SeedType::SEED_SUNFLOWER || theSeedType == SeedType::SEED_MARIGOLD))
    {
        mPlantHealth *= 2;
    }
    mPlantMaxHealth = mPlantHealth;

    if (mSeedType != SeedType::SEED_FLOWERPOT && IsOnBoard())
    {
        TOD_ASSERT(mBoard);
        Plant* aFlowerPot = mBoard->GetFlowerPotAt(mPlantCol, mRow);
        if (aFlowerPot)
            mApp->ReanimationGet(aFlowerPot->mBodyReanimID)->mAnimRate = 0.0f;
    }
}

int Plant::CalcRenderOrder()
{
    PLANT_ORDER anOrder = PLANT_ORDER::PLANT_ORDER_NORMAL;
    RenderLayer aLayer = RenderLayer::RENDER_LAYER_PLANT;

    SeedType aSeedType = mSeedType;
    if (mSeedType == SeedType::SEED_IMITATER && mImitaterType != SeedType::SEED_NONE)
        aSeedType = mImitaterType;

    if (mApp->IsWallnutBowlingLevel())
    {
        aLayer = RenderLayer::RENDER_LAYER_PROJECTILE;
    }
    else if (aSeedType == SeedType::SEED_PUMPKINSHELL)
    {
        anOrder = PLANT_ORDER::PLANT_ORDER_PUMPKIN;
    }
    else if (IsFlying(aSeedType))
    {
        anOrder = PLANT_ORDER::PLANT_ORDER_FLYER;
    }
    else if (aSeedType == SeedType::SEED_FLOWERPOT || (aSeedType == SeedType::SEED_LILYPAD && mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN))
    {
        anOrder = PLANT_ORDER::PLANT_ORDER_LILYPAD;
    }

    return Board::MakeRenderOrder(aLayer, mRow, anOrder * 5 - mX + 800);
}

void Plant::SetSleeping(bool theIsAsleep)
{
    if (mIsAsleep == theIsAsleep || NotOnGround())
        return;

    mIsAsleep = theIsAsleep;
    if (theIsAsleep)
    {
        float aPosX = mX + 50.0f;
        float aPosY = mY + 40.0f;
        if (mSeedType == SeedType::SEED_FUMESHROOM)
            aPosX += 12.0f;
        else if (mSeedType == SeedType::SEED_SCAREDYSHROOM)
            aPosY -= 20.0f;
        else if (mSeedType == SeedType::SEED_GLOOMSHROOM)
            aPosY -= 12.0f;
        else if (mSeedType == SeedType::SEED_NIGHTCAP || mSeedType == SeedType::SEED_DARKCAP)
            aPosY -= 15.0f;

        Reanimation* aSleepReanim = mApp->AddReanimation(aPosX, aPosY, mRenderOrder + 2, ReanimationType::REANIM_SLEEPING);
        aSleepReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
        aSleepReanim->mAnimRate = RandRangeFloat(6.0f, 8.0f);
        aSleepReanim->mAnimTime = RandRangeFloat(0.0f, 0.9f);
        mSleepingReanimID = mApp->ReanimationGetID(aSleepReanim);
    }
    else
    {
        mApp->RemoveReanimation(mSleepingReanimID);
        mSleepingReanimID = ReanimationID::REANIMATIONID_NULL;
    }

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    if (theIsAsleep)
    {
        if (!IsInPlay() && mSeedType == SeedType::SEED_SUNSHROOM)
        {
            aBodyReanim->SetFramesForLayer("anim_bigsleep");
        }
        else if (aBodyReanim->TrackExists("anim_sleep"))
        {
            float aAnimTime = aBodyReanim->mAnimTime;
            aBodyReanim->StartBlend(20);
            aBodyReanim->SetFramesForLayer("anim_sleep");
            aBodyReanim->mAnimTime = aAnimTime;
        }
        else
        {
            aBodyReanim->mAnimRate = 1.0f;
        }

        EndBlink();
    }
    else
    {
        if (!IsInPlay() && mSeedType == SeedType::SEED_SUNSHROOM)
        {
            aBodyReanim->SetFramesForLayer("anim_bigidle");
        }
        else if (aBodyReanim->TrackExists("anim_idle"))
        {
            float aAnimTime = aBodyReanim->mAnimTime;
            aBodyReanim->StartBlend(20);
            aBodyReanim->SetFramesForLayer("anim_idle");
            aBodyReanim->mAnimTime = aAnimTime;
        }

        if (aBodyReanim->mAnimRate < 2.0f && IsInPlay())
            aBodyReanim->mAnimRate = RandRangeFloat(10.0f, 15.0f);
    }
}

int Plant::GetDamageRangeFlags(PlantWeapon thePlantWeapon)
{
    switch (mSeedType)
    {
    case SeedType::SEED_CACTUS:
    case SeedType::SEED_MAD_CACTUS:
        return thePlantWeapon == PlantWeapon::WEAPON_SECONDARY ? 1 : 2;
    case SeedType::SEED_CHERRYBOMB:
    case SeedType::SEED_JALAPENO:
    case SeedType::SEED_COBCANNON:
    case SeedType::SEED_DOOMSHROOM:
        return 127;
    case SeedType::SEED_MELONPULT:
    case SeedType::SEED_CABBAGEPULT:
    case SeedType::SEED_POISONPULT:
    case SeedType::SEED_KERNELPULT:
    case SeedType::SEED_WINTERMELON:
        return 13;
    case SeedType::SEED_POTATOMINE:
    case SeedType::SEED_RED_POTATO_MINE:
        return 77;
    case SeedType::SEED_SQUASH:
    case SeedType::SEED_BOMBSQUASH:
        return 13;
    case SeedType::SEED_PUFFSHROOM:
    case SeedType::SEED_SEASHROOM:
    case SeedType::SEED_FUMESHROOM:
    case SeedType::SEED_GLOOMSHROOM:
    case SeedType::SEED_CHOMPER:
    case SeedType::SEED_SUPER_CHOMPER:
        return 9;
    case SeedType::SEED_CATTAIL:
        return 11;
    case SeedType::SEED_TANGLEKELP:
        return 5;
    case SeedType::SEED_GIANT_WALLNUT:
        return 17;
    default:
        return 1;
    }
}

bool Plant::IsOnHighGround()
{
    return mBoard && mBoard->mGridSquareType[mPlantCol][mRow] == GridSquareType::GRIDSQUARE_HIGH_GROUND;
}

void Plant::SpikeRockTakeDamage()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

    SpikeweedAttack();

    mPlantHealth -= 50;
    if (mPlantHealth <= 300)
    {
        aBodyReanim->AssignRenderGroupToTrack("bigspike3", RENDER_GROUP_HIDDEN);
    }
    if (mPlantHealth <= 150)
    {
        aBodyReanim->AssignRenderGroupToTrack("bigspike2", RENDER_GROUP_HIDDEN);
    }
    if (mPlantHealth <= 0)
    {
        mApp->PlayFoley(FoleyType::FOLEY_SQUISH);
        Die();
    }
}

void Plant::DoomTakeDamage()
{
    mPlantHealth -= 2000;
    if (mPlantHealth <= 0)
    {
        mApp->PlayFoley(FoleyType::FOLEY_SQUISH);
        Die();
    }
}

bool Plant::IsSpiky()
{
    return mSeedType == SeedType::SEED_SPIKEWEED || mSeedType == SeedType::SEED_SPIKEROCK;
}

void Plant::DoRowAreaDamage(int theDamage, unsigned int theDamageFlags)
{
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        int aDiffY = (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) ? 0 : (aZombie->mRow - mRow);
        if (mSeedType == SeedType::SEED_GLOOMSHROOM)
        {
            int aRowRange = (mGloomMode == GLOOM_RANGER || mGloomMode == GLOOM_RAPID_RANGER) ? 2 : 1;
            if (aDiffY < -aRowRange || aDiffY > aRowRange)
                continue;
        }
        else if (aDiffY)
            continue;

        if (aZombie->mOnHighGround == IsOnHighGround() && aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            if (GetRectOverlap(aAttackRect, aZombieRect) > 0)
            {
                int aDamage = theDamage;
                if (mSeedType == SEED_SPIKEWEED)
                {
                    int aTotalMaxPoints = aZombie->mBodyMaxHealth + aZombie->mHelmMaxHealth + aZombie->mShieldMaxHealth + aZombie->mFlyingMaxHealth;
                    aDamage += FloatRoundToInt((float)aTotalMaxPoints * 0.01f);
                }
                else if (mSeedType == SEED_SPIKEROCK)
                {
                    int aTotalMaxPoints = aZombie->mBodyMaxHealth + aZombie->mHelmMaxHealth + aZombie->mShieldMaxHealth + aZombie->mFlyingMaxHealth;
                    aDamage += FloatRoundToInt((float)aTotalMaxPoints * 0.02f);
                }
                if (mSeedType == SEED_GLOOMSHROOM && mGloomMode == GLOOM_RAPID_RANGER)
                {
                    aDamage *= 2;
                }
                if ((aZombie->mZombieType == ZombieType::ZOMBIE_ZAMBONI || aZombie->mZombieType == ZombieType::ZOMBIE_CATAPULT) &&
                    (TestBit(theDamageFlags, DamageFlags::DAMAGE_SPIKE)))
                {
                    aDamage = 1800;

                    if (mSeedType == SeedType::SEED_SPIKEROCK)
                    {
                        SpikeRockTakeDamage();
                    }
                    else if (mSeedType == SeedType::SEED_DOOM_NUT)
                    {
                        DoomTakeDamage();
                    }
                    else
                    {
                        Die();
                    }
                }

                aZombie->TakeDamage(aDamage, theDamageFlags);
                mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
            }
        }
    }
}

TodParticleSystem* Plant::AddAttachedParticle(int thePosX, int thePosY, int theRenderPosition, ParticleEffect theEffect)
{
    TodParticleSystem* aParticle = mApp->ParticleTryToGet(mParticleID);
    if (aParticle)
        aParticle->ParticleSystemDie();

    TodParticleSystem* aNewParticle = mApp->AddTodParticle(thePosX, thePosY, theRenderPosition, theEffect);
    if (aNewParticle)
        mParticleID = mApp->ParticleGetID(aNewParticle);

    return aNewParticle;
}

bool Plant::FindTargetAndFire(int theRow, PlantWeapon thePlantWeapon)
{
    Zombie* aZombie = FindTargetZombie(theRow, thePlantWeapon);
    if (aZombie == nullptr)
    {
        if ((mSeedType == SeedType::SEED_SPLITPEA || mSeedType == SeedType::SEED_SUPER_SPLITPEA) && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
        {
            if (FindTargetZombie(theRow, PlantWeapon::WEAPON_PRIMARY) == nullptr)
                return false;
        }
        else
        {
            return false;
        }
    }

    EndBlink();
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mHeadReanimID);

    if ((mSeedType == SeedType::SEED_SPLITPEA || mSeedType == SeedType::SEED_SUPER_SPLITPEA) && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
    {
        Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);
        aHeadReanim2->StartBlend(20);
        aHeadReanim2->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
        aHeadReanim2->mAnimRate = 35.0f;
        aHeadReanim2->SetFramesForLayer("anim_splitpea_shooting");
        mShootingCounter = 26;
    }
    else if (aHeadReanim && aHeadReanim->TrackExists("anim_shooting"))
    {
        PlantWeaponDef aWeaponDef = GetPlantWeaponDef(mSeedType);
        aHeadReanim->StartBlend(20);
        aHeadReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
        aHeadReanim->mAnimRate = aWeaponDef.mAnimSpeed;
        aHeadReanim->SetFramesForLayer("anim_shooting");
        mShootingCounter = aWeaponDef.mAnimFrameDelay;
    }
    else if (mSeedType == SeedType::SEED_CACTUS || mSeedType == SeedType::SEED_MAD_CACTUS)
    {
        if (mState == PlantState::STATE_CACTUS_HIGH)
        {
            PlayBodyReanim("anim_shootinghigh", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 35.0f);
            mShootingCounter = 23;
        }
        else
        {
            PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 35.0f);
            mShootingCounter = 26;
        }
    }
    else if (mSeedType == SeedType::SEED_GLOOMSHROOM)
    {
        PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 14.0f);
        mShootingCounter = 200;
    }
    else if (mSeedType == SeedType::SEED_CATTAIL)
    {
        PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 30.0f);
        mShootingCounter = 50;
    }
    else if (aBodyReanim && aBodyReanim->TrackExists("anim_shooting"))
    {
        PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 35.0f);

        switch (mSeedType)
        {
        case SeedType::SEED_FUMESHROOM:     mShootingCounter = 50;  break;
        case SeedType::SEED_PUFFSHROOM:     mShootingCounter = 29;  break;
        case SeedType::SEED_SCAREDYSHROOM:  mShootingCounter = 25;  break;
        case SeedType::SEED_CABBAGEPULT:    
        case SeedType::SEED_POISONPULT:
            mShootingCounter = 32;  break;
        case SeedType::SEED_MELONPULT:
        case SeedType::SEED_WINTERMELON:    mShootingCounter = 36;  break;
        case SeedType::SEED_KERNELPULT:
        {
            int aChance = Sexy::Rand(100);
            if (aChance < 6) // 5% chance for big butter
            {
                aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
                aBodyReanim->AssignRenderGroupToPrefix("Cornpult_butter", RENDER_GROUP_NORMAL);
                aBodyReanim->AssignRenderGroupToPrefix("Cornpult_kernal", RENDER_GROUP_HIDDEN);
                mState = PlantState::STATE_KERNELPULT_BIG_BUTTER;
            }
            else if (aChance < 31) // 25% chance for normal butter (30 - 5 = 25)
            {
                aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
                aBodyReanim->AssignRenderGroupToPrefix("Cornpult_butter", RENDER_GROUP_NORMAL);
                aBodyReanim->AssignRenderGroupToPrefix("Cornpult_kernal", RENDER_GROUP_HIDDEN);
                mState = PlantState::STATE_KERNELPULT_BUTTER;
            }

            mShootingCounter = 30;
            break;
        }
        case SeedType::SEED_CACTUS:       
        case SeedType::SEED_MAD_CACTUS:
            mShootingCounter = 35;  break;

        default:                            mShootingCounter = 29;  break;
        }
    }
    else
        Fire(aZombie, theRow, thePlantWeapon);

    return true;
}

void Plant::LaunchThreepeater()
{
    // Logic for the Black Threepeater's special attack
    if (mSeedType == SeedType::SEED_BLACK_THREEPEATER && mBlackThreepeaterShotCount >= 4)
    {
        mBlackThreepeaterShotCount = 0;
        mBlackThreepeaterSpecialBurstCount = 3;
        mBlackThreepeaterSpecialBurstTimer = 1;
        mShootingCounter = 50;
        return;
    }

    // This is the key change: Make BOTH plants perform the normal firing sequence.
    if (mSeedType == SeedType::SEED_THREEPEATER || mSeedType == SeedType::SEED_BLACK_THREEPEATER)
    {
        int rowAbove = mRow - 1;
        int rowBelow = mRow + 1;

        if ((FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY)) ||
            (mBoard->RowCanHaveZombies(rowAbove) && FindTargetZombie(rowAbove, PlantWeapon::WEAPON_PRIMARY)) ||
            (mBoard->RowCanHaveZombies(rowBelow) && FindTargetZombie(rowBelow, PlantWeapon::WEAPON_PRIMARY)))
        {
            if (mSeedType == SeedType::SEED_BLACK_THREEPEATER)
            {
                mBlackThreepeaterShotCount++;
            }

            Reanimation* aHeadReanim1 = mApp->ReanimationGet(mHeadReanimID);
            Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);
            Reanimation* aHeadReanim3 = mApp->ReanimationGet(mHeadReanimID3);

            if (mBoard->RowCanHaveZombies(rowBelow))
            {
                aHeadReanim1->StartBlend(10);
                aHeadReanim1->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
                aHeadReanim1->mAnimRate = 20.0f;
                aHeadReanim1->SetFramesForLayer("anim_shooting1");
            }

            aHeadReanim2->StartBlend(10);
            aHeadReanim2->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
            aHeadReanim2->mAnimRate = 20.0f;
            aHeadReanim2->SetFramesForLayer("anim_shooting2");

            if (mBoard->RowCanHaveZombies(rowAbove))
            {
                aHeadReanim3->StartBlend(10);
                aHeadReanim3->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
                aHeadReanim3->mAnimRate = 20.0f;
                aHeadReanim3->SetFramesForLayer("anim_shooting3");
            }

            mShootingCounter = 35;
        }
    }
}

bool Plant::FindStarFruitTarget()
{
    if (mRecentlyEatenCountdown > 0)
        return true;

    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
    int aCenterStarX = mX + 40;
    int aCenterStarY = mY + 40;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (aZombie->IsDeadOrDying())
            continue;

        if (aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS && mPlantCol >= 5)
                return true;

            Rect aZombieRect = aZombie->GetZombieRect();

            // 1. Same row: Check if zombie is to the LEFT (behind Starfruit)
            if (aZombie->mRow == mRow)
            {
                if (aZombieRect.mX + aZombieRect.mWidth < aCenterStarX)
                    return true;
            }
            else
            {
                if (aZombie->mZombieType == ZombieType::ZOMBIE_DIGGER)
                    aZombieRect.mX += 10;

                // Estimate projectile travel time to zombie
                float aDist = Distance2D((float)aCenterStarX, (float)aCenterStarY, (float)(aZombieRect.mX + aZombieRect.mWidth / 2), (float)(aZombieRect.mY + aZombieRect.mHeight / 2));
                float aProjectileTime = aDist / 3.33f;
                int aZombieHitX = aZombie->ZombieTargetLeadX(aProjectileTime);

                // 2. Straight Up / Straight Down (Star 1: 90 deg, Star 2: -90 deg)
                if ((aZombieHitX + aZombieRect.mWidth >= aCenterStarX - 25) && (aZombieHitX <= aCenterStarX + 25))
                {
                    return true;
                }

                // 3. Angled Down-Right (+30 deg) and Up-Right (-30 deg) (Star 3 & Star 4)
                int aCenterZombieX = aZombieHitX + aZombieRect.mWidth / 2;
                int aCenterZombieY = aZombieRect.mY + aZombieRect.mHeight / 2;
                float angle = RAD_TO_DEG(atan2((float)(aCenterZombieY - aCenterStarY), (float)(aCenterZombieX - aCenterStarX)));

                if (aZombie->mRow > mRow)
                {
                    // Zombie in row below: angle should be positive (+30 deg direction)
                    if (angle >= 10.0f && angle <= 50.0f)
                        return true;
                }
                else if (aZombie->mRow < mRow)
                {
                    // Zombie in row above: angle should be negative (-30 deg direction)
                    if (angle <= -10.0f && angle >= -50.0f)
                        return true;
                }
            }
        }
    }

    return false;
}

void Plant::LaunchStarFruit()
{
    if (FindStarFruitTarget())
    {
        PlayBodyReanim("anim_shoot", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 28.0f);
        mShootingCounter = 40;
    }
}

void Plant::StarFruitFire()
{
    mApp->PlayFoley(FoleyType::FOLEY_THROW);

    float aShootAngleX = cos(DEG_TO_RAD(30.0f)) * 3.33f;
    float aShootAngleY = sin(DEG_TO_RAD(30.0f)) * 3.33f;
    for (int i = 0; i < 5; i++)
    {
        ProjectileType aProjectileType = ProjectileType::PROJECTILE_STAR;
        // Add a 20% chance to fire a Red Star instead
        if (Rand(5) == 0)
        {
            aProjectileType = ProjectileType::PROJECTILE_RED_STAR;
        }

        Projectile* aProjectile = mBoard->AddProjectile(mX + 25, mY + 25, mRenderOrder - 1, mRow, aProjectileType);
        aProjectile->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
        aProjectile->mMotionType = ProjectileMotion::MOTION_STAR;

        switch (i)
        {
        case 0:     aProjectile->mVelX = -3.33f;            aProjectile->mVelY = 0.0f;              break;
        case 1:     aProjectile->mVelX = 0.0f;              aProjectile->mVelY = 3.33f;             break;
        case 2:     aProjectile->mVelX = 0.0f;              aProjectile->mVelY = -3.33f;            break;
        case 3:     aProjectile->mVelX = aShootAngleX;      aProjectile->mVelY = aShootAngleY;      break;
        case 4:     aProjectile->mVelX = aShootAngleX;      aProjectile->mVelY = -aShootAngleY;     break;
        default:    TOD_ASSERT();                                                                   break;
        }
    }
}

void Plant::LaunchSourStarfruit()
{
    if (FindStarFruitTarget()) // Use the same targeting as the regular Starfruit
    {
        mSourStarfruitShotCount++;
        if (mSourStarfruitShotCount >= 5)
        {
            FireSourStarfruitSpecial();
            mSourStarfruitShotCount = 0; // Reset the counter
        }
        else
        {
            PlayBodyReanim("anim_shoot", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 28.0f);
        }
        mShootingCounter = 40;
    }
}

void Plant::FireSourStarfruitSpecial()
{
    mApp->PlayFoley(FoleyType::FOLEY_JUICY); // A nice sound for the special attack

    float aOriginX = mX + 40;
    float aOriginY = mY + 40;
    float aProjectileSpeed = 3.33f;

    // Layer 1: 24 stars (faster ring, 15 degree increments)
    for (int i = 0; i < 24; i++)
    {
        float anAngle = DEG_TO_RAD(360.0f * i / 24.0f);
        Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder - 1, mRow, ProjectileType::PROJECTILE_STAR);
        if (aProjectile)
        {
            aProjectile->mMotionType = ProjectileMotion::MOTION_SPREAD;
            aProjectile->mVelX = cos(anAngle) * aProjectileSpeed;
            aProjectile->mVelY = sin(anAngle) * aProjectileSpeed;
            aProjectile->mDamageRangeFlags = GetDamageRangeFlags(WEAPON_PRIMARY);
        }
    }

    // Layer 2: 24 stars (slower ring, offset by 7.5 degrees)
    for (int i = 0; i < 24; i++)
    {
        float anAngle = DEG_TO_RAD(360.0f * i / 24.0f + 7.5f);
        Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder - 1, mRow, ProjectileType::PROJECTILE_STAR);
        if (aProjectile)
        {
            aProjectile->mMotionType = ProjectileMotion::MOTION_SPREAD;
            aProjectile->mVelX = cos(anAngle) * (aProjectileSpeed * 0.75f);
            aProjectile->mVelY = sin(anAngle) * (aProjectileSpeed * 0.75f);
            aProjectile->mDamageRangeFlags = GetDamageRangeFlags(WEAPON_PRIMARY);
        }
    }
}

void Plant::FireSourStarfruit()
{
    mApp->PlayFoley(FoleyType::FOLEY_THROW);

    float aShootAngleX = cos(DEG_TO_RAD(30.0f)) * 3.33f;
    float aShootAngleY = sin(DEG_TO_RAD(30.0f)) * 3.33f;
    for (int i = 0; i < 5; i++)
    {
        ProjectileType aProjectileType = ProjectileType::PROJECTILE_RED_STAR;

        Projectile* aProjectile = mBoard->AddProjectile(mX + 25, mY + 25, mRenderOrder - 1, mRow, aProjectileType);
        aProjectile->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
        aProjectile->mMotionType = ProjectileMotion::MOTION_STAR;

        switch (i)
        {
        case 0:     aProjectile->mVelX = -3.33f;            aProjectile->mVelY = 0.0f;              break;
        case 1:     aProjectile->mVelX = 0.0f;              aProjectile->mVelY = 3.33f;             break;
        case 2:     aProjectile->mVelX = 0.0f;              aProjectile->mVelY = -3.33f;            break;
        case 3:     aProjectile->mVelX = aShootAngleX;      aProjectile->mVelY = aShootAngleY;      break;
        case 4:     aProjectile->mVelX = aShootAngleX;      aProjectile->mVelY = -aShootAngleY;     break;
        default:    TOD_ASSERT();                                                                   break;
        }
    }
}

void Plant::UpdateShooter()
{
    if (mChilledCounter > 0 && mBoard->mMainCounter % 2 == 1)
        return;

    PlantWeaponDef aWeaponDef = GetPlantWeaponDef(mSeedType);

    // --- Active Burst Sub-Shot Processing ---
    if (mShootingBurstCount > 0 && mShootingCounter == 0)
    {
        if (aWeaponDef.mPattern == PATTERN_THREE_ROW)
        {
            LaunchThreepeater();
        }
        else if (aWeaponDef.mPattern == PATTERN_STAR_5WAY)
        {
            if (mSeedType == SeedType::SEED_SOUR_STARFRUIT)
                LaunchSourStarfruit();
            else
                LaunchStarFruit();
        }
        else if (mSeedType == SeedType::SEED_CACTUS || mSeedType == SeedType::SEED_MAD_CACTUS)
        {
            if (mState == PlantState::STATE_CACTUS_HIGH)
                FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
            else if (mState == PlantState::STATE_CACTUS_LOW)
                FindTargetAndFire(mRow, PlantWeapon::WEAPON_SECONDARY);
        }
        else
        {
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
            if (aWeaponDef.mPattern == PATTERN_SPLIT_BACK)
            {
                FindTargetAndFire(mRow, PlantWeapon::WEAPON_SECONDARY);
            }
        }

        mShootingBurstCount--;
    }

    // --- Primary Launch Countdown ---
    mLaunchCounter--;
    if (mIsBoosted)
    {
        mLaunchCounter--;
        mLaunchCounter--;
    }
    if (mLaunchCounter <= 0)
    {
        int aNewLaunchRate = mLaunchRate;

        if (mSeedType == SEED_GLOOMSHROOM)
        {
            if (mGloomMode == GLOOM_RAPID || mGloomMode == GLOOM_RAPID_RANGER)
            {
                aNewLaunchRate = 133;
            }
        }

        if (mSeedType == SeedType::SEED_SPLITPEA || mSeedType == SeedType::SEED_SUPER_SPLITPEA)
        {
            bool aTargetInFront = (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY) != nullptr);
            bool aTargetBehind = false;
            Rect aBackRect = GetPlantAttackRect(PlantWeapon::WEAPON_SECONDARY);
            Zombie* aZombieIter = nullptr;
            while (mBoard->IterateZombies(aZombieIter))
            {
                if (aZombieIter->mRow == mRow && aZombieIter->EffectedByDamage(GetDamageRangeFlags(PlantWeapon::WEAPON_SECONDARY)))
                {
                    Rect aZombieRect = aZombieIter->GetZombieRect();
                    if (GetRectOverlap(aBackRect, aZombieRect) > 0)
                    {
                        aTargetBehind = true;
                        break;
                    }
                }
            }

            if (aTargetBehind) aNewLaunchRate /= 3;
            else if (aTargetInFront) aNewLaunchRate *= 1.5;
        }

        if (mIsOnPotAndLily)
        {
            aNewLaunchRate *= 0.75f;
        }
        mLaunchCounter = aNewLaunchRate - Sexy::Rand(15);

        // Check if any target exists before starting burst
        bool hasTarget = false;
        if (aWeaponDef.mPattern == PATTERN_THREE_ROW)
        {
            hasTarget = (FindTargetZombie(mRow - 1, WEAPON_PRIMARY) || FindTargetZombie(mRow, WEAPON_PRIMARY) || FindTargetZombie(mRow + 1, WEAPON_PRIMARY));
        }
        else if (aWeaponDef.mPattern == PATTERN_SPLIT_BACK)
        {
            hasTarget = (FindTargetZombie(mRow, WEAPON_PRIMARY) || FindTargetZombie(mRow, WEAPON_SECONDARY));
        }
        else if (aWeaponDef.mPattern == PATTERN_TARGETED_STRAIGHT || aWeaponDef.mPattern == PATTERN_HOMING || mSeedType == SeedType::SEED_CATTAIL)
        {
            hasTarget = (FindTargetZombie(mRow, WEAPON_PRIMARY) != nullptr);
        }
        else if (mSeedType == SeedType::SEED_CACTUS || mSeedType == SeedType::SEED_MAD_CACTUS)
        {
            hasTarget = (FindTargetZombie(mRow, WEAPON_PRIMARY) || FindTargetZombie(mRow, WEAPON_SECONDARY));
        }
        else
        {
            hasTarget = (FindTargetZombie(mRow, WEAPON_PRIMARY) != nullptr);
        }

        if (hasTarget)
        {
            if (mSeedType == SeedType::SEED_SNOWPEA)
            {
                int aChance = Rand(100);
                if (aChance < 10)       mShootingBurstCount = 3; // 10% chance for 3 shots
                else if (aChance < 60)  mShootingBurstCount = 2; // 50% chance for 2 shots
                else                    mShootingBurstCount = 1; // 40% chance for 1 shot
            }
            else
            {
                mShootingBurstCount = aWeaponDef.mShotCount;
            }
            if (mShootingBurstCount < 1) mShootingBurstCount = 1;
            mShootingBurstTimer = 0;
        }
    }
}

bool Plant::MakesSun()
{
    return mSeedType == SeedType::SEED_SUNFLOWER || mSeedType == SeedType::SEED_TWINSUNFLOWER || mSeedType == SeedType::SEED_SUNSHROOM;
}

void Plant::UpdateProductionPlant()
{
    if (!IsInPlay() || mApp->IsIZombieLevel() || mApp->mGameMode == GameMode::GAMEMODE_UPSELL || mApp->mGameMode == GameMode::GAMEMODE_INTRO)
        return;

    if (mBoard->HasLevelAwardDropped())
        return;

    if (mSeedType == SeedType::SEED_MARIGOLD && mBoard->mCurrentWave == mBoard->mNumWaves)
    {
        if (mState != PlantState::STATE_MARIGOLD_ENDING)
        {
            mState = PlantState::STATE_MARIGOLD_ENDING;
            mStateCountdown = 6000;
        }
        else if (mStateCountdown <= 0)
            return;
    }

    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_LAST_STAND && mBoard->mChallenge->mChallengeState != ChallengeState::STATECHALLENGE_LAST_STAND_ONSLAUGHT)
        return;

    mLaunchCounter--;
    if (mIsBoosted)
    {
        mLaunchCounter--; // 100%
        mLaunchCounter--; // 100%
        mLaunchCounter--; // 100%
        mLaunchCounter--; // 100%
        mLaunchCounter--; // 100%
        mLaunchCounter--; // 100%
        mLaunchCounter--; // 100%
        mLaunchCounter--; // 100%
        mLaunchCounter--; // 100%
        mLaunchCounter--; // 100%
        //each 'mLaunchCounter--;' will boost by 100%
    }
    if (mLaunchCounter <= 100)
    {
        int aFlashCountdown = TodAnimateCurve(100, 0, mLaunchCounter, 0, 100, TodCurves::CURVE_LINEAR);
        mEatenFlashCountdown = max(mEatenFlashCountdown, aFlashCountdown);
    }
    if (mLaunchCounter <= 0)
    {
        int aLaunchRate = mLaunchRate;
        if (mIsOnPotAndLily)
        {
            aLaunchRate *= 0.75f; // 25% faster
        }
        mLaunchCounter = RandRangeInt(mLaunchRate - 150, mLaunchRate);
        mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);

        if (mSeedType == SeedType::SEED_SUNSHROOM)
        {
            if (mState == PlantState::STATE_SUNSHROOM_SMALL)
            {
                mBoard->AddCoin(mX, mY, CoinType::COIN_SMALLSUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            }
            else
            {
                // 1% chance to produce a White Sun
                if (Rand(100) < 1)
                {
                    mBoard->AddCoin(mX, mY, CoinType::COIN_WHITE_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
                }
                else // 99% chance for a normal sun
                {
                    mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
                }
            }
        }
        else if (mSeedType == SeedType::SEED_SUNFLOWER)
        {
            mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
        }
        else if (mSeedType == SeedType::SEED_TWINSUNFLOWER)
        {
            mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
        }
        else if (mSeedType == SeedType::SEED_MARIGOLD)
        {
            mBoard->AddCoin(mX, mY, (Sexy::Rand(100) < 10) ? CoinType::COIN_GOLD : CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
        }

        if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME)
        {
            if (mSeedType == SeedType::SEED_SUNFLOWER)
            {
                mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            }
            else if (mSeedType == SeedType::SEED_MARIGOLD)
            {
                mBoard->AddCoin(mX, mY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
            }
        }
    }
}

void Plant::UpdateSunShroom()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (mState == PlantState::STATE_SUNSHROOM_SMALL)
    {
        if (mStateCountdown == 0)
        {
            PlayBodyReanim("anim_grow", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 12.0f);
            mState = PlantState::STATE_SUNSHROOM_GROWING;
            mApp->PlayFoley(FoleyType::FOLEY_PLANTGROW);
        }

        UpdateProductionPlant();
    }
    else if (mState == PlantState::STATE_SUNSHROOM_GROWING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayBodyReanim("anim_bigidle", ReanimLoopType::REANIM_LOOP, 10, RandRangeFloat(12.0f, 15.0f));
            mState = PlantState::STATE_SUNSHROOM_BIG;
        }
    }
    else
    {
        UpdateProductionPlant();
    }
}

void Plant::UpdateGraveBuster()
{
    if (mState == PlantState::STATE_GRAVEBUSTER_LANDING)
    {
        if (mApp->ReanimationGet(mBodyReanimID)->mLoopCount > 0)
        {
            PlayBodyReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 10, 12.0f);
            mStateCountdown = 400;
            mState = PlantState::STATE_GRAVEBUSTER_EATING;
            AddAttachedParticle(mX + 40, mY + 40, mRenderOrder + 4, ParticleEffect::PARTICLE_GRAVE_BUSTER);
        }
    }
    else if (mState == PlantState::STATE_GRAVEBUSTER_EATING && mStateCountdown == 0)
    {
        // Check if we targeted a gravestone
        if (mGraveBusterGraveTarget != nullptr)
        {
            mGraveBusterGraveTarget->GridItemDie();
            mBoard->mGravesCleared++;
            mBoard->DropLootPiece(mX + 40, mY, 12);
        }
        // Otherwise, check if we targeted a plant
        else if (mGraveBusterPlantTarget != nullptr)
        {
            // Your sun spawning logic
            int aPlantCost = GetCost(mGraveBusterPlantTarget->mSeedType, mGraveBusterPlantTarget->mImitaterType);
            int aNumSuns = (aPlantCost / 25) + 2;

            for (int i = 0; i < aNumSuns; i++)
            {
                mBoard->AddCoin(mGraveBusterPlantTarget->mX + 15, mGraveBusterPlantTarget->mY + 15, COIN_SUN, COIN_MOTION_FROM_PLANT);
            }
            mGraveBusterPlantTarget->Die();
        }

        mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 4, ParticleEffect::PARTICLE_GRAVE_BUSTER_DIE);
        Die();
    }
}

void Plant::PlayBodyReanim(const char* theTrackName, ReanimLoopType theLoopType, int theBlendTime, float theAnimRate)
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

    if (theBlendTime > 0)
        aBodyReanim->StartBlend(theBlendTime);
    if (theAnimRate > 0.0f)
    {
        if (mIsOnPotAndLily)
        {
            aBodyReanim->mAnimRate = theAnimRate * 1.25f;
        }
        else
        {
            aBodyReanim->mAnimRate = theAnimRate;
        }
    }

    aBodyReanim->mLoopType = theLoopType;
    aBodyReanim->mLoopCount = 0;
    aBodyReanim->SetFramesForLayer(theTrackName);
}

void Plant::UpdatePotato()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

    if (mState == PlantState::STATE_NOTREADY)
    {
        if (mStateCountdown == 0)
        {
            mApp->AddTodParticle(mX + mWidth / 2, mY + mHeight / 2, mRenderOrder, ParticleEffect::PARTICLE_POTATO_MINE_RISE);
            PlayBodyReanim("anim_rise", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 18.0f);
            mState = PlantState::STATE_POTATO_RISING;
            mApp->PlayFoley(FoleyType::FOLEY_DIRT_RISE);
        }
    }
    else if (mState == PlantState::STATE_POTATO_RISING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            float aRate = RandRangeFloat(12.0f, 15.0f);
            PlayBodyReanim("anim_armed", ReanimLoopType::REANIM_LOOP, 0, aRate);

            Reanimation* aLightReanim = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, GetPlantDefinition(mSeedType).mReanimationType);
            aLightReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aLightReanim->mAnimRate = aRate - 2.0f;
            aLightReanim->SetFramesForLayer("anim_glow");
            aLightReanim->mFrameCount = 10;
            aLightReanim->ShowOnlyTrack("anim_glow");
            aLightReanim->SetTruncateDisappearingFrames("anim_glow", false);
            mLightReanimID = mApp->ReanimationGetID(aLightReanim);
            aLightReanim->AttachToAnotherReanimation(aBodyReanim, "anim_light");

            mState = PlantState::STATE_POTATO_ARMED;
            mBlinkCountdown = 400 + Sexy::Rand(4000);
        }
    }
    else if (mState == PlantState::STATE_POTATO_ARMED)
    {
        if (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY))
        {
            DoSpecial();
        }
        else
        {
            Reanimation* aLightReanim = mApp->ReanimationTryToGet(mLightReanimID);
            if (aLightReanim)
            {
                aLightReanim->mFrameCount = TodAnimateCurve(200, 50, DistanceToClosestZombie(), 10, 3, TodCurves::CURVE_LINEAR);
            }
        }
    }
}

Zombie* Plant::FindSmartTangleKelpTarget()
{
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);
    Zombie* aBestZombie = nullptr;
    int aHighestPoints = -1;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (aZombie->mRow != mRow)
            continue;

        if (aZombie->IsDeadOrDying() || !aZombie->mHasHead || aZombie->IsTangleKelpTarget())
            continue;

        if (!aZombie->EffectedByDamage(GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY)))
            continue;

        Rect aZombieRect = aZombie->GetZombieRect();
        if (GetRectOverlap(aAttackRect, aZombieRect) <= 0)
            continue;

        int aTotalPoints = aZombie->mBodyHealth + aZombie->mHelmHealth + aZombie->mShieldHealth + aZombie->mFlyingHealth;
        if (aZombie->mZombieType == ZombieType::ZOMBIE_GARGANTUAR || aZombie->mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
        {
            aTotalPoints += 3000;
        }

        if (aBestZombie == nullptr || aTotalPoints > aHighestPoints)
        {
            aBestZombie = aZombie;
            aHighestPoints = aTotalPoints;
        }
    }

    return aBestZombie;
}

void Plant::UpdateTanglekelp()
{
    if (mState != PlantState::STATE_TANGLEKELP_GRABBING)
    {
        Zombie* aZombie = FindSmartTangleKelpTarget();
        if (aZombie)
        {
            mApp->PlayFoley(FoleyType::FOLEY_FLOOP);
            mState = PlantState::STATE_TANGLEKELP_GRABBING;
            mStateCountdown = 100;
            aZombie->PoolSplash(false);

            float aVinesPosX = -13.0f;
            float aVinesPosY = 15.0f;
            if (aZombie->mZombieType == ZombieType::ZOMBIE_SNORKEL)
            {
                aVinesPosX = -43.0f;
                aVinesPosY = 55.0f;
            }
            if (aZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_RIDING)
            {
                aVinesPosX = -20.0f;
                aVinesPosY = 37.0f;
            }
            Reanimation* aGrabReanim = aZombie->AddAttachedReanim(aVinesPosX, aVinesPosY, ReanimationType::REANIM_TANGLEKELP);
            if (aGrabReanim)
            {
                aGrabReanim->SetFramesForLayer("anim_grab");
                aGrabReanim->mAnimRate = 24.0f;
                aGrabReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
            }

            mTargetZombieID = mBoard->ZombieGetID(aZombie);
        }
    }
    else
    {
        if (mStateCountdown == 50)
        {
            Zombie* aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
            if (aZombie)
            {
                aZombie->DragUnder();
                aZombie->PoolSplash(false);
            }
        }

        if (mStateCountdown == 20)
        {
            int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
            Reanimation* aSplashReanim = mApp->AddReanimation(mX - 23, mY + 7, aRenderPosition, ReanimationType::REANIM_SPLASH);
            aSplashReanim->OverrideScale(1.3f, 1.3f);

            mApp->AddTodParticle(mX + 31, mY + 64, aRenderPosition, ParticleEffect::PARTICLE_PLANTING_POOL);
            mApp->PlayFoley(FoleyType::FOLEY_ZOMBIE_ENTERING_WATER);
        }

        if (mStateCountdown == 0)
        {
            Die();

            Zombie* aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
            if (aZombie)
            {
                aZombie->DieWithLoot();
            }
        }
    }
}

void Plant::SpikeweedAttack()
{
    TOD_ASSERT(IsSpiky());

    if (mState != PlantState::STATE_SPIKEWEED_ATTACKING)
    {
        PlayBodyReanim("anim_attack", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 18.0f);
        mApp->PlaySample(SOUND_THROW);
        
        mState = PlantState::STATE_SPIKEWEED_ATTACKING;
        mStateCountdown = 100;
    }
}

void Plant::UpdateSpikeweed()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (mState == PlantState::STATE_SPIKEWEED_ATTACKING)
    {
        if (mStateCountdown == 0)
        {
            mState = PlantState::STATE_NOTREADY;
        }
        else if (mSeedType == SeedType::SEED_SPIKEROCK)
        {
            if (mStateCountdown == 69 || mStateCountdown == 33)
            {
                DoRowAreaDamage(20, 33U);
            }
        }
        else if (mStateCountdown == 75)
        {
            DoRowAreaDamage(20, 33U);
        }

        if (aBodyReanim->mLoopCount > 0)
        {
            PlayIdleAnim(RandRangeFloat(12.0f, 15.0f));
        }
    }
    else if (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY))
    {
        SpikeweedAttack();
    }
}

void Plant::UpdateScaredyShroom()
{
    if (mPlantCol >= 6)
    {
        if (mState != PlantState::STATE_SCAREDYSHROOM_SCARED)
        {
            mState = PlantState::STATE_SCAREDYSHROOM_SCARED;
            PlayBodyReanim("anim_scaredidle", ReanimLoopType::REANIM_LOOP, 10, 0.0f);
        }
        mLaunchCounter = mLaunchRate;
        return;
    }

    if (mShootingCounter > 0)
        return;

    bool aHasZombieNearby = false;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        Rect aZombieRect = aZombie->GetZombieRect();
        int aDiffY = (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) ? 0 : (aZombie->mRow - mRow);
        if (!aZombie->mMindControlled && !aZombie->IsDeadOrDying() && aDiffY <= 1 && aDiffY >= -1 && GetCircleRectOverlap(mX, mY + 20.0f, 120, aZombieRect))
        {
            aHasZombieNearby = true;
            break;
        }
    }

    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (mState == PlantState::STATE_READY)
    {
        if (aHasZombieNearby)
        {
            mState = PlantState::STATE_SCAREDYSHROOM_LOWERING;
            PlayBodyReanim("anim_scared", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 10.0f);
        }
    }
    else if (mState == PlantState::STATE_SCAREDYSHROOM_LOWERING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_SCAREDYSHROOM_SCARED;
            PlayBodyReanim("anim_scaredidle", ReanimLoopType::REANIM_LOOP, 10, 0.0f);
        }
    }
    else if (mState == PlantState::STATE_SCAREDYSHROOM_SCARED)
    {
        if (!aHasZombieNearby)
        {
            mState = PlantState::STATE_SCAREDYSHROOM_RAISING;

            float aAnimRate = RandRangeFloat(7.0f, 12.0f);
            PlayBodyReanim("anim_grow", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, aAnimRate);
        }
    }
    else if (mState == PlantState::STATE_SCAREDYSHROOM_RAISING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_READY;

            float aAnimRate = RandRangeFloat(10.0f, 15.0f);
            PlayIdleAnim(aAnimRate);
        }
    }

    if (mState != PlantState::STATE_READY)
    {
        mLaunchCounter = mLaunchRate;
    }
}

void Plant::UpdateTorchwood()
{
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);

    Projectile* aProjectile = nullptr;
    while (mBoard->IterateProjectiles(aProjectile))
    {
        if (mSeedType == SEED_TORCHWOOD)
        {
            if ((aProjectile->mRow == mRow) &&
                (aProjectile->mProjectileType == ProjectileType::PROJECTILE_PEA ||
                    aProjectile->mProjectileType == ProjectileType::PROJECTILE_SNOWPEA ||
                    aProjectile->mProjectileType == ProjectileType::PROJECTILE_FIREBALL ||
                    aProjectile->mProjectileType == ProjectileType::PROJECTILE_ICEPEA ||
                    aProjectile->mProjectileType == ProjectileType::PROJECTILE_RED_FIRE_PEA ||
                    aProjectile->mProjectileType == ProjectileType::PROJECTILE_BLUE_FIRE_PEA ||
                    aProjectile->mProjectileType == ProjectileType::PROJECTILE_BOUNCING_PEA))
            {
                Rect aProjectileRect = aProjectile->GetProjectileRect();
                if (GetRectOverlap(aAttackRect, aProjectileRect) >= 10)
                {
                    if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_BLUE_FIRE_PEA)
                    {
                        aProjectile->ConvertToWhiteFirePea(mPlantCol);
                    }
                    else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_RED_FIRE_PEA)
                    {
                        aProjectile->ConvertToBlueFirePea(mPlantCol);
                    }
                    else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
                    {
                        aProjectile->ConvertToRedFirePea(mPlantCol);
                    }
                    else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_PEA)
                    {
                        aProjectile->ConvertToFireball(mPlantCol);
                    }
                    else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
                    {
                        aProjectile->ConvertToPea(mPlantCol);
                    }
                    else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_ICEPEA)
                    {
                        aProjectile->ConvertToBlueFirePea(mPlantCol);
                    }
                    else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_BOUNCING_PEA)
                    {
                        aProjectile->ConvertToBlueFirePea(mPlantCol);
                    }
                }
            }
        } 
        else if (mSeedType == SEED_BLUE_TORCHWOOD)
        {
            if ((aProjectile->mRow == mRow) &&
                (aProjectile->mProjectileType == ProjectileType::PROJECTILE_PEA ||
                    aProjectile->mProjectileType == ProjectileType::PROJECTILE_SNOWPEA ||
                    aProjectile->mProjectileType == ProjectileType::PROJECTILE_FIREBALL ||
                    aProjectile->mProjectileType == ProjectileType::PROJECTILE_ICEPEA ||
                    aProjectile->mProjectileType == ProjectileType::PROJECTILE_RED_FIRE_PEA ||
                    aProjectile->mProjectileType == ProjectileType::PROJECTILE_BOUNCING_PEA))
            {
                Rect aProjectileRect = aProjectile->GetProjectileRect();
                if (GetRectOverlap(aAttackRect, aProjectileRect) >= 10)
                {
                    aProjectile->ConvertToBlueFirePea(mPlantCol);
                }
            }
            else if ((aProjectile->mRow == mRow) &&
                (aProjectile->mProjectileType == ProjectileType::PROJECTILE_BLUE_FIRE_PEA ||
                    aProjectile->mProjectileType == ProjectileType::PROJECTILE_WHITE_FIRE_PEA))
            {
                Rect aProjectileRect = aProjectile->GetProjectileRect();
                if (GetRectOverlap(aAttackRect, aProjectileRect) >= 10)
                {
                    if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_BLUE_FIRE_PEA)
                    {
                        aProjectile->ConvertToWhiteFirePea(mPlantCol);
                    }
                    else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_WHITE_FIRE_PEA)
                    {
                        aProjectile->ConvertToBlackFirePea(mPlantCol);
                    }
                }
            }
        }
    }
}

void Plant::DoSquashDamage()
{
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if ((aZombie->mRow == mRow || aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) && aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            if (GetRectOverlap(aAttackRect, aZombieRect) > ((aZombie->mZombieType == ZombieType::ZOMBIE_FOOTBALL || aZombie->mZombieType == ZombieType::ZOMBIE_GIGA_FOOTBALL) ? -20 : 0))
            {
                aZombie->TakeDamage(1800, 18U);
            }
        }
    }
}

Zombie* Plant::FindSquashTarget()
{
    Rect aAttackRectFront(mX + 20, mY, 180, mHeight); // A slightly larger front box
    Rect aAttackRectBack(mX - 40, mY, 60, mHeight);         // A new box for behind

    Zombie* aClosestZombie = nullptr;
    int aClosestDist = 0;
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (aZombie->mRow != mRow && aZombie->mZombieType != ZOMBIE_BOSS)
            continue;
        if (!aZombie->mHasHead || aZombie->IsTangleKelpTarget() || !aZombie->EffectedByDamage(aDamageRangeFlags))
            continue;

        // Skip zombies that are already vaulting or being targeted by another Squash
        if (aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || aZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_IN_JUMP ||
            aZombie->IsSquashTarget(this))
            continue;

        Rect aZombieRect = aZombie->GetZombieRect();
        bool inFront = GetRectOverlap(aAttackRectFront, aZombieRect) > 0;
        bool inBack = GetRectOverlap(aAttackRectBack, aZombieRect) > 0;

        // If the zombie is in either the front or back attack zones
        if (inFront || inBack)
        {
            // Calculate the actual distance from the center of the Squash to the center of the zombie
            int aDist = abs((aZombieRect.mX + aZombieRect.mWidth / 2) - (mX + mWidth / 2));

            // If this is the first target found, or if this zombie is closer than the previous one
            if (aClosestZombie == nullptr || aDist < aClosestDist)
            {
                aClosestZombie = aZombie;
                aClosestDist = aDist;
            }
        }
    }
    return aClosestZombie;
}

Zombie* Plant::FindBombSquashTarget()
{
    // 3 tiles forward (240px) and 1 tile back (80px)
    Rect aAttackRectFront(mX + 20, mY, 260, mHeight);
    Rect aAttackRectBack(mX - 80, mY, 100, mHeight);

    Zombie* aClosestZombie = nullptr;
    int aClosestDist = 0;
    int aDamageRangeFlags = GetDamageRangeFlags(WEAPON_PRIMARY);

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (aZombie->mRow != mRow && aZombie->mZombieType != ZOMBIE_BOSS)
            continue;
        if (!aZombie->mHasHead || aZombie->IsTangleKelpTarget() || !aZombie->EffectedByDamage((unsigned int)aDamageRangeFlags))
            continue;
        if (aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || aZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_IN_JUMP || aZombie->IsSquashTarget(this))
            continue;

        Rect aZombieRect = aZombie->GetZombieRect();
        if (GetRectOverlap(aAttackRectFront, aZombieRect) > 0 || GetRectOverlap(aAttackRectBack, aZombieRect) > 0)
        {
            int aDist = abs((aZombieRect.mX + aZombieRect.mWidth / 2) - (mX + mWidth / 2));
            if (aClosestZombie == nullptr || aDist < aClosestDist)
            {
                aClosestZombie = aZombie;
                aClosestDist = aDist;
            }
        }
    }
    return aClosestZombie;
}

void Plant::UpdateSquash()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    TOD_ASSERT(aBodyReanim);

    if (mState == PlantState::STATE_NOTREADY)
    {
        Zombie* aZombie = FindSquashTarget();
        if (aZombie)
        {
            mTargetZombieID = mBoard->ZombieGetID(aZombie);
            mTargetX = aZombie->ZombieTargetLeadX(0.0f) - mWidth / 2;
            mState = PlantState::STATE_SQUASH_LOOK;
            mStateCountdown = 80;
            PlayBodyReanim(mTargetX < mX ? "anim_lookleft" : "anim_lookright", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 24.0f);
            mApp->PlayFoley(FoleyType::FOLEY_SQUASH_HMM);
        }
    }
    else if (mState == PlantState::STATE_SQUASH_LOOK)
    {
        if (mStateCountdown <= 0)
        {
            PlayBodyReanim("anim_jumpup", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
            mState = PlantState::STATE_SQUASH_PRE_LAUNCH;
            mStateCountdown = 30;
        }
    }
    else if (mState == PlantState::STATE_SQUASH_PRE_LAUNCH)
    {
        if (mStateCountdown <= 0)
        {
            Zombie* aZombie = FindSquashTarget();
            if (aZombie)
            {
                mTargetX = aZombie->ZombieTargetLeadX(30.0f) - mWidth / 2;
            }

            mState = PlantState::STATE_SQUASH_RISING;
            mStateCountdown = 50;
            mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
        }
    }
    else
    {
        int aTargetCol = mBoard->PixelToGridXKeepOnBoard(mTargetX, mY);
        int aDestY = mBoard->GridToPixelY(aTargetCol, mRow) + 8;

        if (mState == PlantState::STATE_SQUASH_RISING)
        {
            mX = TodAnimateCurve(50, 20, mStateCountdown, mBoard->GridToPixelX(mPlantCol, mStartRow), mTargetX, TodCurves::CURVE_EASE_IN_OUT);
            mY = TodAnimateCurve(50, 20, mStateCountdown, mBoard->GridToPixelY(mPlantCol, mStartRow), aDestY - 120, TodCurves::CURVE_EASE_IN_OUT);

            if (mStateCountdown == 0)
            {
                PlayBodyReanim("anim_jumpdown", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 60.0f);
                mState = PlantState::STATE_SQUASH_FALLING;
                mStateCountdown = 10;
            }
        }
        else if (mState == PlantState::STATE_SQUASH_FALLING)
        {
            mY = TodAnimateCurve(10, 0, mStateCountdown, aDestY - 120, aDestY, TodCurves::CURVE_EASE_IN_OUT);

            if (mStateCountdown == 5)
            {
                DoSquashDamage();
            }

            if (mStateCountdown == 0)
            {
                if (mBoard->IsPoolSquare(aTargetCol, mRow))
                {
                    mApp->AddReanimation(mX - 11, mY + 20, mRenderOrder + 1, ReanimationType::REANIM_SPLASH);
                    mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
                    mApp->PlaySample(SOUND_ZOMBIESPLASH);

                    Die();
                }
                else
                {
                    mState = PlantState::STATE_SQUASH_DONE_FALLING;
                    mStateCountdown = 100;

                    mBoard->ShakeBoard(1, 4);
                    mApp->PlayFoley(FoleyType::FOLEY_THUMP);
                    float aOffsetY = mBoard->StageHasRoof() ? 69.0f : 80.0f;
                    mApp->AddTodParticle(mX + 40, mY + aOffsetY, mRenderOrder + 4, ParticleEffect::PARTICLE_DUST_SQUASH);
                }
            }
        }
        else if (mState == PlantState::STATE_SQUASH_DONE_FALLING)
        {
            mSquashJumpCount++; // Increment the jump counter

            // If this was the first jump, reset the Squash to attack again
            if (mSquashJumpCount < 2)
            {
                mState = PlantState::STATE_NOTREADY;
                mStateCountdown = 100;
                mTargetZombieID = ZOMBIEID_NULL; 

                mPlantCol = mBoard->PixelToGridX(mX, mY);
                mRow = mStartRow;
                mRenderOrder = CalcRenderOrder();
            
                PlayIdleAnim(gReanimatorDefArray[(int)GetPlantDefinition(mSeedType).mReanimationType].mFPS);
            }
            else // If this was the second jump, the Squash disappears
            {
                if (mStateCountdown == 0)
                {
                    Die();
                }
            }
        }
    }
}

void Plant::UpdateBombSquash()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    TOD_ASSERT(aBodyReanim);

    if (mState == PlantState::STATE_NOTREADY)
    {
        Zombie* aZombie = FindBombSquashTarget();
        if (aZombie)
        {
            mTargetZombieID = mBoard->ZombieGetID(aZombie);
            mTargetX = aZombie->ZombieTargetLeadX(0.0f) - mWidth / 2;
            mState = PlantState::STATE_SQUASH_LOOK;
            mStateCountdown = 80;
            PlayBodyReanim(mTargetX < mX ? "anim_lookleft" : "anim_lookright", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 24.0f);
            mApp->PlayFoley(FoleyType::FOLEY_SQUASH_HMM);
        }
    }
    else if (mState == PlantState::STATE_SQUASH_LOOK)
    {
        if (mStateCountdown <= 0)
        {
            PlayBodyReanim("anim_jumpup", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
            mState = PlantState::STATE_SQUASH_PRE_LAUNCH;
            mStateCountdown = 30;
        }
    }
    else if (mState == PlantState::STATE_SQUASH_PRE_LAUNCH)
    {
        if (mStateCountdown <= 0)
        {
            Zombie* aZombie = FindBombSquashTarget();
            if (aZombie)
            {
                mTargetX = aZombie->ZombieTargetLeadX(30.0f) - mWidth / 2;
            }
            mState = PlantState::STATE_SQUASH_RISING;
            mStateCountdown = 50;
            mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
        }
    }
    else
    {
        int aTargetCol = mBoard->PixelToGridXKeepOnBoard(mTargetX, mY);
        int aDestY = mBoard->GridToPixelY(aTargetCol, mRow) + 8;

        if (mState == PlantState::STATE_SQUASH_RISING)
        {
            mX = TodAnimateCurve(50, 20, mStateCountdown, mBoard->GridToPixelX(mPlantCol, mStartRow), mTargetX, TodCurves::CURVE_EASE_IN_OUT);
            mY = TodAnimateCurve(50, 20, mStateCountdown, mBoard->GridToPixelY(mPlantCol, mStartRow), aDestY - 120, TodCurves::CURVE_EASE_IN_OUT);

            if (mStateCountdown == 0)
            {
                PlayBodyReanim("anim_jumpdown", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 60.0f);
                mState = PlantState::STATE_SQUASH_FALLING;
                mStateCountdown = 10;
            }
        }
        else if (mState == PlantState::STATE_SQUASH_FALLING)
        {
            mY = TodAnimateCurve(10, 0, mStateCountdown, aDestY - 120, aDestY, TodCurves::CURVE_LINEAR);

            if (mStateCountdown == 5)
            {
                mBoard->DamageAllZombiesInRadius(mRow, mX + 40, mY + 40, 115, 900, 1, false, 1U << DAMAGE_BYPASSES_SHIELD);
                mBoard->ShakeBoard(3, -4);
                mApp->PlayFoley(FoleyType::FOLEY_EXPLOSION);
                mApp->AddTodParticle(mX + 40, mY + 40, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POWIE);
            }

            if (mStateCountdown == 0)
            {
                if (mBoard->IsPoolSquare(aTargetCol, mRow))
                {
                    mApp->AddReanimation(mX - 11, mY + 20, mRenderOrder + 1, ReanimationType::REANIM_SPLASH);
                    mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
                    mApp->PlaySample(SOUND_ZOMBIESPLASH);
                    Die();
                }
                else
                {
                    mState = PlantState::STATE_SQUASH_DONE_FALLING;
                    mStateCountdown = 100;
                    mBoard->ShakeBoard(1, 4);
                    mApp->PlayFoley(FoleyType::FOLEY_THUMP);
                    float aOffsetY = mBoard->StageHasRoof() ? 69.0f : 80.0f;
                    mApp->AddTodParticle(mX + 40, mY + aOffsetY, mRenderOrder + 4, ParticleEffect::PARTICLE_DUST_SQUASH);
                }
            }
        }
        else if (mState == PlantState::STATE_SQUASH_DONE_FALLING)
        {
            mSquashJumpCount++;
            if (mSquashJumpCount < 3)
            {
                mState = PlantState::STATE_NOTREADY;
                mStateCountdown = 100;
                mTargetZombieID = ZOMBIEID_NULL;
                mPlantCol = mBoard->PixelToGridX(mX, mY);
                mRow = mStartRow;
                mRenderOrder = CalcRenderOrder();
                PlayIdleAnim(gReanimatorDefArray[(int)GetPlantDefinition(mSeedType).mReanimationType].mFPS);
            }
            else
            {
                if (mStateCountdown == 0)
                {
                    Die();
                }
            }
        }
    }
}

void Plant::UpdateDoomShroom()
{
    if (mIsAsleep || mState == PlantState::STATE_DOINGSPECIAL)
        return;

    mState = PlantState::STATE_DOINGSPECIAL;
    mDoSpecialCountdown = 100;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    TOD_ASSERT(aBodyReanim);

    aBodyReanim->SetFramesForLayer("anim_explode");
    aBodyReanim->mAnimRate = 23.0f;
    aBodyReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
    aBodyReanim->SetShakeOverride("DoomShroom_head1", 1.0f);
    aBodyReanim->SetShakeOverride("DoomShroom_head2", 2.0f);
    aBodyReanim->SetShakeOverride("DoomShroom_head3", 2.0f);
    mApp->PlayFoley(FoleyType::FOLEY_REVERSE_EXPLOSION);
}

void Plant::UpdateIceShroom()
{
    if (!mIsAsleep && mState != PlantState::STATE_DOINGSPECIAL)
    {
        mState = PlantState::STATE_DOINGSPECIAL;
        mDoSpecialCountdown = 100;
    }
}

void Plant::UpdateBlover()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (aBodyReanim->mLoopCount > 0 && aBodyReanim->mLoopType != ReanimLoopType::REANIM_LOOP)
    {
        aBodyReanim->SetFramesForLayer("anim_loop");
        aBodyReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
    }

    if (mState != PlantState::STATE_DOINGSPECIAL && mStateCountdown == 0)
    {
        DoSpecial();
    }
}

void Plant::UpdateFlowerPot()
{
    if (mState == PlantState::STATE_FLOWERPOT_INVULNERABLE && mStateCountdown == 0)
        mState = PlantState::STATE_NOTREADY;
}

void Plant::UpdateLilypad()
{
    if (mState == PlantState::STATE_LILYPAD_INVULNERABLE && mStateCountdown == 0)
        mState = PlantState::STATE_NOTREADY;
}

void Plant::UpdateCoffeeBean()
{
    if (mState == PlantState::STATE_DOINGSPECIAL)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->mLoopCount > 0)
        {
            Die();
        }
    }
}

void Plant::UpdateUmbrella()
{
    if (mState == PlantState::STATE_UMBRELLA_TRIGGERED)
    {
        if (mStateCountdown == 0)
        {
            mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PROJECTILE, mRow + 1, 0);
            mState = PlantState::STATE_UMBRELLA_REFLECTING;
        }
    }
    else if (mState == PlantState::STATE_UMBRELLA_REFLECTING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayIdleAnim(0.0f);
            mState = PlantState::STATE_NOTREADY;
            mRenderOrder = CalcRenderOrder();
        }
    }
}

void Plant::UpdateCobCannon()
{
    if (mState == PlantState::STATE_COBCANNON_ARMING)
    {
        if (mStateCountdown == 0)
        {
            mState = PlantState::STATE_COBCANNON_LOADING;
            PlayBodyReanim("anim_charge", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
        }
    }
    else if (mState == PlantState::STATE_COBCANNON_LOADING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->ShouldTriggerTimedEvent(0.5f))
        {
            mApp->PlayFoley(FoleyType::FOLEY_SHOOP);
        }
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_COBCANNON_READY;
            PlayIdleAnim(12.0f);
        }
    }
    else if (mState == PlantState::STATE_COBCANNON_READY)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        ReanimatorTrackInstance* aCobTrack = aBodyReanim->GetTrackInstanceByName("CobCannon_cob");
        aCobTrack->mTrackColor = GetFlashingColor(mBoard->mMainCounter, 75);
    }
    else if (mState == PlantState::STATE_COBCANNON_FIRING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->ShouldTriggerTimedEvent(0.48f))
        {
            mApp->PlayFoley(FoleyType::FOLEY_COB_LAUNCH);
        }
    }
}

void Plant::UpdateCactus()
{
    if (mShootingCounter > 0)
        return;

    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (mState == PlantState::STATE_CACTUS_RISING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_CACTUS_HIGH;
            PlayBodyReanim("anim_idlehigh", ReanimLoopType::REANIM_LOOP, 20, 0.0f);
            if (mApp->IsIZombieLevel())
            {
                aBodyReanim->mAnimRate = 0;
            }

            mLaunchCounter = 1;
        }
    }
    else if (mState == PlantState::STATE_CACTUS_HIGH)
    {
        if (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY) == nullptr)
        {
            mState = PlantState::STATE_CACTUS_LOWERING;
            PlayBodyReanim("anim_lower", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, aBodyReanim->mDefinition->mFPS);
        }
    }
    else if (mState == PlantState::STATE_CACTUS_LOWERING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_CACTUS_LOW;
            PlayIdleAnim(0.0f);
        }
    }
    else if (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY))
    {
        mState = PlantState::STATE_CACTUS_RISING;
        PlayBodyReanim("anim_rise", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, aBodyReanim->mDefinition->mFPS);
        mApp->PlayFoley(FoleyType::FOLEY_PLANTGROW);
    }
}

void Plant::UpdateChomper()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (mState == PlantState::STATE_READY)
    {
        if (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY))
        {
            PlayBodyReanim("anim_bite", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
            mState = PlantState::STATE_CHOMPER_BITING;
            mStateCountdown = 70;
        }
    }
    else if (mState == PlantState::STATE_CHOMPER_BITING)
    {
        if (mStateCountdown == 0)
        {
            mApp->PlayFoley(FoleyType::FOLEY_BIGCHOMP);

            Zombie* aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
            bool doBite = false;
            if (aZombie)
            {
                if (aZombie->mZombieType == ZombieType::ZOMBIE_GARGANTUAR || 
                    aZombie->mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR || 
                    aZombie->mZombieType == ZombieType::ZOMBIE_BOSS ||
                    aZombie->mZombieType == ZombieType::ZOMBIE_IMP ||
                    aZombie->mZombieType == ZombieType::ZOMBIE_YETI ||
                    aZombie->mZombieType == ZombieType::ZOMBIE_WALLNUT_HEAD ||
                    aZombie->mZombieType == ZombieType::ZOMBIE_CACHED_POLEVAULTER_WITH_POLE ||
                    aZombie->mZombieType == ZombieType::ZOMBIE_NORMAL ||
                    aZombie->mZombieType == ZombieType::ZOMBIE_NEWSPAPER ||
                    aZombie->mZombieType == ZombieType::ZOMBIE_TRAFFIC_CONE ||
                    aZombie->mZombieType == ZombieType::ZOMBIE_ZAMBONI ||
                    aZombie->mZombieType == ZombieType::ZOMBIE_TALLNUT_HEAD ||
                    aZombie->mZombieType == ZombieType::ZOMBIE_DOOR ||
                    aZombie->mZombieType == ZombieType::ZOMBIE_LADDER ||
                    aZombie->mZombieType == ZombieType::ZOMBIE_FLAG ||
                    aZombie->mZombieType == ZombieType::ZOMBIE_POLEVAULTER ||
                    aZombie->mZombieType == ZombieType::ZOMBIE_DOOR_CONEHEAD ||
                    aZombie->mZombieType == ZombieType::ZOMBIE_DOOR_BUCKETHEAD ||
                    aZombie->mZombieType == ZombieType::ZOMBIE_JACK_IN_THE_BOX ||
                    aZombie->mZombieType == ZombieType::ZOMBIE_GIGA_FOOTBALL
                    )
                {
                    doBite = true;
                }
            }
            bool doMiss = false;
            if (aZombie == nullptr)
            {
                doMiss = true;
            }
            else if (!aZombie->IsImmobilizied())
            {
                if (aZombie->IsBouncingPogo() ||
                    aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT)
                {
                    doMiss = true;
                }
            }

            if (aZombie && !doBite && !doMiss && aZombie->mHelmType == HelmType::HELMTYPE_NONE && aZombie->mShieldType == ShieldType::SHIELDTYPE_NONE)
            {
                // It's a plain zombie, just bite it instead of eating it.
                mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
                aZombie->TakeDamage(1800, 0U);
                mState = PlantState::STATE_CHOMPER_BITING_MISSED;
            }
            else if (doBite)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
                aZombie->TakeDamage(120, 0U);
                mState = PlantState::STATE_CHOMPER_BITING_MISSED;
            }
            else if (doMiss)
            {
                mState = PlantState::STATE_CHOMPER_BITING_MISSED;
            }
            else
            {
                aZombie->DieWithLoot();
                mState = PlantState::STATE_CHOMPER_BITING_GOT_ONE;
            }
        }
    }
    else if (mState == PlantState::STATE_CHOMPER_BITING_GOT_ONE)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayBodyReanim("anim_chew", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
            if (mApp->IsIZombieLevel())
            {
                aBodyReanim->mAnimRate = 0;
            }

            mState = PlantState::STATE_CHOMPER_DIGESTING;
            mStateCountdown = 4000;
        }
    }
    else if (mState == PlantState::STATE_CHOMPER_DIGESTING)
    {
        if (mStateCountdown == 0)
        {
            PlayBodyReanim("anim_swallow", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
            mState = PlantState::STATE_CHOMPER_SWALLOWING;
        }
    }
    else if ((mState == PlantState::STATE_CHOMPER_SWALLOWING || mState == PlantState::STATE_CHOMPER_BITING_MISSED) && aBodyReanim->mLoopCount > 0)
    {
        PlayIdleAnim(aBodyReanim->mDefinition->mFPS);
        mState = PlantState::STATE_READY;
    }
}

void Plant::UpdateSuperChomper()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (mState == PlantState::STATE_READY)
    {
        Zombie* aZombie = FindSuperChomperTarget();
        if (aZombie)
        {
            PlayBodyReanim("anim_bite", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
            mState = PlantState::STATE_CHOMPER_BITING;
            mStateCountdown = 70; // Time until the bite happens
        }
    }
    else if (mState == PlantState::STATE_CHOMPER_BITING)
    {
        if (mStateCountdown == 0)
        {
            mApp->PlayFoley(FoleyType::FOLEY_BIGCHOMP);

            Zombie* aZombie = FindSuperChomperTarget();
            if (aZombie)
            {
                // Calculate total max health
                int totalMaxHealth = aZombie->mBodyMaxHealth + aZombie->mHelmMaxHealth + aZombie->mShieldMaxHealth;

                // Determine damage percentage
                float damagePercentage = (totalMaxHealth > 1000) ? 0.20f : 0.10f;

                // Calculate and deal damage
                int damageToDeal = totalMaxHealth * damagePercentage;
                aZombie->TakeDamage(damageToDeal, 0U);
            }

            // After biting, go into a short cooldown instead of digesting
            mState = PlantState::STATE_CHOMPER_BITING_MISSED;
        }
    }
    else if (mState == PlantState::STATE_CHOMPER_BITING_MISSED)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayIdleAnim(aBodyReanim->mDefinition->mFPS);
            mState = PlantState::STATE_READY;
        }
    }
}

MagnetItem* Plant::GetFreeMagnetItem()
{
    if (mSeedType == SeedType::SEED_GOLD_MAGNET)
    {
        for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
        {
            if (mMagnetItems[i].mItemType == MagnetItemType::MAGNET_ITEM_NONE)
            {
                return &mMagnetItems[i];
            }
        }

        return nullptr;
    }

    return &mMagnetItems[0];
}

void Plant::MagnetShroomAttactItem(Zombie* theZombie)
{
    mState = PlantState::STATE_MAGNETSHROOM_SUCKING;
    PlayBodyReanim("anim_attract", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 18.0f);
    mApp->PlayFoley(FoleyType::FOLEY_MAGNETSHROOM);

    MagnetItem* aMagnetItem = GetFreeMagnetItem();
    if (theZombie->mHelmType == HelmType::HELMTYPE_PAIL)
    {
        int aDamageIndex = theZombie->GetHelmDamageIndex();

        theZombie->mHelmHealth = 0;
        theZombie->mHelmType = HelmType::HELMTYPE_NONE;
        theZombie->GetTrackPosition("anim_bucket", aMagnetItem->mPosX, aMagnetItem->mPosY);
        theZombie->ReanimShowPrefix("anim_bucket", RENDER_GROUP_HIDDEN);
        theZombie->ReanimShowPrefix("anim_hair", RENDER_GROUP_NORMAL);

        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_BUCKET1->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_BUCKET1->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 25.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_PAIL_1 + aDamageIndex);
    }
    else if (theZombie->mHelmType == HelmType::HELMTYPE_FOOTBALL)
    {
        int aDamageIndex = theZombie->GetHelmDamageIndex();

        theZombie->mHelmHealth = 0;
        theZombie->mHelmType = HelmType::HELMTYPE_NONE;
        theZombie->GetTrackPosition("zombie_football_helmet", aMagnetItem->mPosX, aMagnetItem->mPosY);
        theZombie->ReanimShowPrefix("zombie_football_helmet", RENDER_GROUP_HIDDEN);
        theZombie->ReanimShowPrefix("anim_hair", RENDER_GROUP_NORMAL);

        aMagnetItem->mPosX += 37.0f;
        aMagnetItem->mPosY -= 60.0f;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_FOOTBALL_HELMET_1 + aDamageIndex);
    }
    else if (theZombie->mHelmType == HelmType::HELMTYPE_BLACK_CONE)
    {
        int aDamageIndex = theZombie->GetHelmDamageIndex();
        theZombie->mHelmHealth = 0;
        theZombie->mHelmType = HelmType::HELMTYPE_NONE;
        theZombie->GetTrackPosition("anim_cone", aMagnetItem->mPosX, aMagnetItem->mPosY);
        theZombie->ReanimShowPrefix("anim_cone", RENDER_GROUP_HIDDEN);
        theZombie->ReanimShowPrefix("anim_hair", RENDER_GROUP_NORMAL);
        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_BLACK_CONE1->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_BLACK_CONE1->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 25.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_BLACK_CONE_1 + aDamageIndex);
    }
    else if (theZombie->mHelmType == HelmType::HELMTYPE_BLACK_PAIL)
    {
        int aDamageIndex = theZombie->GetHelmDamageIndex();
        theZombie->mHelmHealth = 0;
        theZombie->mHelmType = HelmType::HELMTYPE_NONE;
        theZombie->GetTrackPosition("anim_bucket", aMagnetItem->mPosX, aMagnetItem->mPosY);
        theZombie->ReanimShowPrefix("anim_bucket", RENDER_GROUP_HIDDEN);
        theZombie->ReanimShowPrefix("anim_hair", RENDER_GROUP_NORMAL);
        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_BLACK_BUCKET1->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_BLACK_BUCKET1->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 25.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_BLACK_PAIL_1 + aDamageIndex);
    }
    else if (theZombie->mShieldType == ShieldType::SHIELDTYPE_BLACK_DOOR)
    {
        int aDamageIndex = theZombie->GetShieldDamageIndex();
        theZombie->DetachShield();
        theZombie->mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
        if (!theZombie->mIsEating)
        {
            TOD_ASSERT(theZombie->mZombieHeight == ZombieHeight::HEIGHT_ZOMBIE_NORMAL);
            theZombie->StartWalkAnim(0);
        }
        theZombie->GetTrackPosition("anim_screendoor", aMagnetItem->mPosX, aMagnetItem->mPosY);
        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_BLACK_SCREENDOOR1->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_BLACK_SCREENDOOR1->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 30.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f);
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_BLACK_DOOR_1 + aDamageIndex);
    }
    else if (theZombie->mShieldType == ShieldType::SHIELDTYPE_DOOR)
    {
        int aDamageIndex = theZombie->GetShieldDamageIndex();

        theZombie->DetachShield();
        theZombie->mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
        if (!theZombie->mIsEating)
        {
            TOD_ASSERT(theZombie->mZombieHeight == ZombieHeight::HEIGHT_ZOMBIE_NORMAL);
            theZombie->StartWalkAnim(0);
        }
        theZombie->GetTrackPosition("anim_screendoor", aMagnetItem->mPosX, aMagnetItem->mPosY);

        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_SCREENDOOR1->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_SCREENDOOR1->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 30.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f);
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_DOOR_1 + aDamageIndex);
    }
    else if (theZombie->mShieldType == ShieldType::SHIELDTYPE_LADDER)
    {
        int aDamageIndex = theZombie->GetShieldDamageIndex();

        theZombie->DetachShield();

        aMagnetItem->mPosX = theZombie->mPosX + 31.0f;
        aMagnetItem->mPosY = theZombie->mPosY + 20.0f;
        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_LADDER_5->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_LADDER_5->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 30.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f);
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_LADDER_1 + aDamageIndex);
    }
    else if (theZombie->mZombieType == ZombieType::ZOMBIE_POGO)
    {
        theZombie->PogoBreak(16U);
        // ZombieDrawPosition aDrawPos;
        // theZombie->GetDrawPos(aDrawPos);
        theZombie->GetTrackPosition("Zombie_pogo_stick", aMagnetItem->mPosX, aMagnetItem->mPosY);

        aMagnetItem->mPosX += 40.0f - IMAGE_REANIM_ZOMBIE_LADDER_5->GetWidth() / 2;
        aMagnetItem->mPosY += 84.0f - IMAGE_REANIM_ZOMBIE_LADDER_5->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 30.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f);
        aMagnetItem->mItemType = theZombie->mHasArm ? MagnetItemType::MAGNET_ITEM_POGO_1 : MagnetItemType::MAGNET_ITEM_POGO_3;
    }
    else if (theZombie->mZombiePhase == ZombiePhase::PHASE_JACK_IN_THE_BOX_RUNNING)
    {
        theZombie->StopZombieSound();
        theZombie->PickRandomSpeed();
        theZombie->mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
        theZombie->ReanimShowPrefix("Zombie_jackbox_box", RENDER_GROUP_HIDDEN);
        theZombie->ReanimShowPrefix("Zombie_jackbox_handle", RENDER_GROUP_HIDDEN);
        theZombie->GetTrackPosition("Zombie_jackbox_box", aMagnetItem->mPosX, aMagnetItem->mPosY);

        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_JACKBOX_BOX->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_JACKBOX_BOX->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 15.0f;
        aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_JACK_IN_THE_BOX;
    }
    else if (theZombie->mZombieType == ZombieType::ZOMBIE_DIGGER)
    {
        theZombie->DiggerLoseAxe();
        theZombie->GetTrackPosition("Zombie_digger_pickaxe", aMagnetItem->mPosX, aMagnetItem->mPosY);

        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_DIGGER_PICKAXE->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_DIGGER_PICKAXE->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 45.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 15.0f;
        aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_PICK_AXE;
    }
}

bool Plant::DrawMagnetItemsOnTop()
{
    if (mSeedType == SeedType::SEED_GOLD_MAGNET)
    {
        for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
        {
            if (mMagnetItems[i].mItemType != MagnetItemType::MAGNET_ITEM_NONE)
            {
                return true;
            }
        }

        return false;
    }

    if (mSeedType == SeedType::SEED_MAGNETSHROOM)
    {
        for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
        {
            MagnetItem* aMagnetItem = &mMagnetItems[i];
            if (aMagnetItem->mItemType != MagnetItemType::MAGNET_ITEM_NONE)
            {
                SexyVector2 aVectorToPlant(mX + aMagnetItem->mDestOffsetX - aMagnetItem->mPosX, mY + aMagnetItem->mDestOffsetY - aMagnetItem->mPosY);
                if (aVectorToPlant.Magnitude() > 20.0f)
                {
                    return true;
                }
            }
        }

        return false;
    }
}

void Plant::UpdateMagnetShroom()
{
    // Handle the countdown and item pulling visuals
    for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
    {
        MagnetItem* aMagnetItem = &mMagnetItems[i];
        if (aMagnetItem->mItemType != MagnetItemType::MAGNET_ITEM_NONE)
        {
            SexyVector2 aVectorToPlant(mX + aMagnetItem->mDestOffsetX - aMagnetItem->mPosX, mY + aMagnetItem->mDestOffsetY - aMagnetItem->mPosY);
            if (aVectorToPlant.Magnitude() > 20.0f)
            {
                aMagnetItem->mPosX += aVectorToPlant.x * 0.05f;
                aMagnetItem->mPosY += aVectorToPlant.y * 0.05f;
            }
        }
    }

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);

    // STATE: Sucking an item towards the plant
    if (mState == STATE_MAGNETSHROOM_SUCKING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayBodyReanim("anim_nonactive_idle", ReanimLoopType::REANIM_LOOP, 20, 2.0f);
            if (mApp->IsIZombieLevel())
            {
                aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
                aBodyReanim->mAnimRate = 0.0f;
            }
            mState = STATE_MAGNETSHROOM_CHARGING;
            mStateCountdown = 300; // Cooldown before it can fire
        }
    }
    // STATE: Holding an item and waiting for cooldown/target
    else if (mState == STATE_MAGNETSHROOM_CHARGING)
    {
        if (mStateCountdown > 0)
        {
            mStateCountdown--;
        }
        else // Cooldown is over, ready to fire
        {
            Zombie* aTargetZombie = FindTargetZombie(mRow, WEAPON_PRIMARY);
            if (aTargetZombie != nullptr)
            {
                mState = STATE_MAGNETSHROOM_FIRING; // Use the new FIRING state
                PlayBodyReanim("anim_shooting", REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
                FireMagnet(aTargetZombie);
            }
        }
    }
    // STATE: Firing the held item
    else if (mState == STATE_MAGNETSHROOM_FIRING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            // After firing, clear the item and return to the ready state
            mMagnetItems[0].mItemType = MAGNET_ITEM_NONE;
            mState = STATE_READY;
            PlayIdleAnim(14.0f);
        }
    }
    // STATE: Ready to find a new target to steal from
    else if (mState == STATE_READY)
    {
        Zombie* aClosestZombie = nullptr;
        float aClosestDistance = 0.0f;

        // Find a zombie with metal to steal
        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if (aZombie->mIsMagnetized) continue;

            int aDiffY = aZombie->mRow - mRow;
            Rect aZombieRect = aZombie->GetZombieRect();

            if (aZombie->mMindControlled) continue;
            if (!aZombie->mHasHead) continue;
            if (aZombie->mZombieHeight != ZombieHeight::HEIGHT_ZOMBIE_NORMAL || aZombie->mZombiePhase == ZombiePhase::PHASE_RISING_FROM_GRAVE) continue;
            if (aZombie->IsDeadOrDying()) continue;
            if (aZombieRect.mX > BOARD_WIDTH || aDiffY > 2 || aDiffY < -2) continue;

            if (aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING ||
                aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_STUNNED ||
                aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_WALKING ||
                aZombie->mZombieType == ZombieType::ZOMBIE_POGO)
            {
                if (!aZombie->mHasObject) continue;
            }
            else if (aZombie->mZombieType == ZombieType::ZOMBIE_FLAG ||
                !(aZombie->mHelmType == HelmType::HELMTYPE_PAIL ||
                aZombie->mHelmType == HelmType::HELMTYPE_FOOTBALL ||
                aZombie->mHelmType == HelmType::HELMTYPE_TRAFFIC_CONE ||
                aZombie->mHelmType == HelmType::HELMTYPE_BLACK_CONE ||
                aZombie->mHelmType == HelmType::HELMTYPE_BLACK_PAIL ||
                aZombie->mShieldType == ShieldType::SHIELDTYPE_DOOR ||
                aZombie->mShieldType == ShieldType::SHIELDTYPE_BLACK_DOOR ||
                aZombie->mShieldType == ShieldType::SHIELDTYPE_LADDER ||
                aZombie->mZombiePhase == ZombiePhase::PHASE_JACK_IN_THE_BOX_RUNNING))
                continue;

            int aRadius = aZombie->mIsEating ? 320 : 270;
            if (GetCircleRectOverlap(mX, mY + 20, aRadius, aZombieRect))
            {
                float aDistance = Distance2D(mX, mY, aZombieRect.mX, aZombieRect.mY);
                aDistance += abs(aDiffY) * 80.0f;

                if (aClosestZombie == nullptr || aDistance < aClosestDistance)
                {
                    aClosestZombie = aZombie;
                    aClosestDistance = aDistance;
                }
            }
        }

        if (aClosestZombie)
        {
            aClosestZombie->mIsMagnetized = true;
            MagnetShroomAttactItem(aClosestZombie);
            return;
        }

        float aClosestLadderDist = 0.0f;
        GridItem* aClosestLadder = nullptr;

        GridItem* aGridItem = nullptr;
        while (mBoard->IterateGridItems(aGridItem))
        {
            if (aGridItem->mGridItemType == GridItemType::GRIDITEM_LADDER)
            {
                int aDiffX = abs(aGridItem->mGridX - mPlantCol);
                int aDiffY = abs(aGridItem->mGridY - mRow);
                int aSquareDistance = max(aDiffX, aDiffY);
                if (aSquareDistance <= 2)
                {
                    float aDistance = aSquareDistance + aDiffY * 0.05f;
                    if (aClosestLadder == nullptr || aDistance < aClosestLadderDist)
                    {
                        aClosestLadder = aGridItem;
                        aClosestLadderDist = aDistance;
                    }
                }
            }
        }

        if (aClosestLadder)
        {
            mState = PlantState::STATE_MAGNETSHROOM_SUCKING;
            mStateCountdown = 1500;
            PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
            mApp->PlayFoley(FoleyType::FOLEY_MAGNETSHROOM);
            aClosestLadder->GridItemDie();
            MagnetItem* aMagnetItem = GetFreeMagnetItem();
            aMagnetItem->mPosX = mBoard->GridToPixelX(aClosestLadder->mGridX, aClosestLadder->mGridY) + 40;
            aMagnetItem->mPosY = mBoard->GridToPixelY(aClosestLadder->mGridX, aClosestLadder->mGridY);
            aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 10.0f;
            aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f);
            aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_LADDER_PLACED;
        }
    }
}

void Plant::FireMagnet(Zombie* theTargetZombie)
{
    mApp->PlayFoley(FOLEY_THROW);
    int aOriginX = mX + 19;
    int aOriginY = mY - 37;

    Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder - 1, mRow, PROJECTILE_ARMOR);
    aProjectile->mDamageRangeFlags = GetDamageRangeFlags(WEAPON_PRIMARY);
    aProjectile->mArmorType = mMagnetItems[0].mItemType;

    // Set the motion type to straight and give it some spin for visual effect
    aProjectile->mMotionType = MOTION_STRAIGHT;
    aProjectile->mRotationSpeed = RandRangeFloat(-0.08f, 0.08f);
}

Coin* Plant::FindGoldMagnetTarget()
{
    Coin* aClosestCoin = nullptr;
    float aClosestDistance = 0.0f;

    Coin* aCoin = nullptr;
    while (mBoard->IterateCoins(aCoin))
    {
        if ((aCoin->IsMoney() || aCoin->IsSun()) && aCoin->mCoinMotion != CoinMotion::COIN_MOTION_FROM_PRESENT && !aCoin->mIsBeingCollected && aCoin->mCoinAge >= 50)
        {
            float aDistance = Distance2D(mX + mWidth / 2, mY + mHeight / 2, aCoin->mPosX + aCoin->mWidth / 2, aCoin->mPosY + aCoin->mHeight / 2);
            if (aClosestCoin == nullptr || aDistance < aClosestDistance)
            {
                aClosestCoin = aCoin;
                aClosestDistance = aDistance;
            }
        }
    }

    return aClosestCoin;
}

void Plant::GoldMagnetFindTargets()
{
    if (GetFreeMagnetItem() == nullptr)
    {
        TOD_ASSERT();
        return;
    }

    for (;;)
    {
        MagnetItem* aMagnetItem = GetFreeMagnetItem();
        if (aMagnetItem == nullptr)
            break;

        Coin* aCoin = FindGoldMagnetTarget();
        if (aCoin == nullptr)
            break;

        aMagnetItem->mPosX = aCoin->mPosX + 15.0f;
        aMagnetItem->mPosY = aCoin->mPosY + 15.0f;
        aMagnetItem->mDestOffsetX = RandRangeFloat(20.0f, 40.0f);
        aMagnetItem->mDestOffsetY = RandRangeFloat(-20.0f, 0.0f) + 20.0f;

        switch (aCoin->mType)
        {
        case CoinType::COIN_SILVER:     aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_SILVER_COIN;   break;
        case CoinType::COIN_GOLD:       aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_GOLD_COIN;     break;
        case CoinType::COIN_DIAMOND:    aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_DIAMOND;       break;
        case CoinType::COIN_SMALLSUN:   aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_SMALL_SUN;     break;
        case CoinType::COIN_SUN:        aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_NORMAL_SUN;    break;
        case CoinType::COIN_LARGESUN:   aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_LARGE_SUN;     break;
        case CoinType::COIN_WHITE_SUN:  aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_WHITE_SUN;     break;
        case CoinType::COIN_BIG_WHITE_SUN:  aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_BIG_WHITE_SUN;     break;
        default:                        TOD_ASSERT();                                                       return;
        }
        
        aCoin->Die();
    }
}

bool Plant::IsAGoldMagnetAboutToSuck()
{
    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (!aPlant->NotOnGround() && aPlant->mSeedType == SeedType::SEED_GOLD_MAGNET && aPlant->mState == PlantState::STATE_MAGNETSHROOM_SUCKING)
        {
            Reanimation* aBodyReanim = mApp->ReanimationGet(aPlant->mBodyReanimID);
            if (aBodyReanim->mAnimTime < 0.5f)
            {
                return true;
            }
        }
    }

    return false;
}

void Plant::UpdateGoldMagnetShroom()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

    bool aIsSuckingCoin = false;
    for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
    {
        MagnetItem* aMagnetItem = &mMagnetItems[i];
        if (aMagnetItem->mItemType != MagnetItemType::MAGNET_ITEM_NONE)
        {
            SexyVector2 aVectorToPlant(mX + aMagnetItem->mDestOffsetX - aMagnetItem->mPosX, mY + aMagnetItem->mDestOffsetY - aMagnetItem->mPosY);
            float aDistance = aVectorToPlant.Magnitude();
            if (aDistance < 20.0f)
            {
                if (aMagnetItem->mItemType == MAGNET_ITEM_SMALL_SUN || aMagnetItem->mItemType == MAGNET_ITEM_NORMAL_SUN || aMagnetItem->mItemType == MAGNET_ITEM_LARGE_SUN || aMagnetItem->mItemType == MAGNET_ITEM_WHITE_SUN || aMagnetItem->mItemType == MAGNET_ITEM_BIG_WHITE_SUN)
                {
                    // It's a sun
                    int aSunValue = 0;
                    if (aMagnetItem->mItemType == MAGNET_ITEM_SMALL_SUN) aSunValue = 15;
                    else if (aMagnetItem->mItemType == MAGNET_ITEM_NORMAL_SUN) aSunValue = 25;
                    else if (aMagnetItem->mItemType == MAGNET_ITEM_LARGE_SUN) aSunValue = 50;
                    else if (aMagnetItem->mItemType == MAGNET_ITEM_WHITE_SUN) aSunValue = 100;
                    else if (aMagnetItem->mItemType == MAGNET_ITEM_BIG_WHITE_SUN) aSunValue = 100;
                    
                    mBoard->AddSunMoney(aSunValue * 2); // Add doubled value
                    mApp->PlayFoley(FoleyType::FOLEY_SUN);
                }
                else
                {
                    // It's money
                    CoinType aCoinType;
                    if (aMagnetItem->mItemType == MAGNET_ITEM_SILVER_COIN) aCoinType = COIN_SILVER;
                    else if (aMagnetItem->mItemType == MAGNET_ITEM_GOLD_COIN) aCoinType = COIN_GOLD;
                    else aCoinType = COIN_DIAMOND;

                    int aValue = Coin::GetCoinValue(aCoinType);
                    mApp->mPlayerInfo->AddCoins(aValue * 2); // Add doubled value
                    mBoard->mCoinsCollected += aValue * 2;
                    mApp->PlayFoley(FoleyType::FOLEY_COIN);
                }

                aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_NONE;
            }
            else
            {
                float aSpeed = TodAnimateCurveFloatTime(30.0f, 0.0f, aDistance, 0.02f, 0.05f, TodCurves::CURVE_LINEAR);
                aMagnetItem->mPosX += aVectorToPlant.x * aSpeed;
                aMagnetItem->mPosY += aVectorToPlant.y * aSpeed;

                aIsSuckingCoin = true;
            }
        }
    }

    if (mState == PlantState::STATE_MAGNETSHROOM_CHARGING)
    {
        if (mStateCountdown == 0)
        {
            mState = PlantState::STATE_READY;
        }
    }
    else if (mState == PlantState::STATE_MAGNETSHROOM_SUCKING)
    {
        if (aBodyReanim->ShouldTriggerTimedEvent(0.4f))
        {
            mApp->PlayFoley(FoleyType::FOLEY_MAGNETSHROOM);
            GoldMagnetFindTargets();
        }

        if (aBodyReanim->mLoopCount > 0 && !aIsSuckingCoin)
        {
            PlayIdleAnim(14.0f);
            mState = PlantState::STATE_MAGNETSHROOM_CHARGING;
            mStateCountdown = RandRangeInt(200, 300);
        }
    }
    else if (!IsAGoldMagnetAboutToSuck() && Sexy::Rand(50) == 0 && FindGoldMagnetTarget())
    {
        mBoard->ShowCoinBank();
        mState = PlantState::STATE_MAGNETSHROOM_SUCKING;
        PlayBodyReanim("anim_attract", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
    }
}

void Plant::RemoveEffects()
{
    mApp->RemoveParticle(mParticleID);
    mApp->RemoveReanimation(mBodyReanimID);
    mApp->RemoveReanimation(mHeadReanimID);
    mApp->RemoveReanimation(mHeadReanimID2);
    mApp->RemoveReanimation(mHeadReanimID3);
    mApp->RemoveReanimation(mLightReanimID);
    mApp->RemoveReanimation(mBlinkReanimID);
    mApp->RemoveReanimation(mSleepingReanimID);
}

void Plant::Squish()
{
    if (mShieldHealth > 0)
    {
        mShieldHealth = 0;
        mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
        return; // Shield absorbs the smash/crush! Plant is saved!
    }

    if (mSeedType == SeedType::SEED_TORCHWOOD || mSeedType == SeedType::SEED_BLUE_TORCHWOOD)
    {
        mApp->PlayFoley(FoleyType::FOLEY_JALAPENO_IGNITE);
        mBoard->DoFwoosh(mRow);
        BurnRow(mRow);
    }

    if (NotOnGround())
        return;

    if (!mIsAsleep)
    {
        if (mSeedType == SeedType::SEED_CHERRYBOMB || mSeedType == SeedType::SEED_JALAPENO ||
            mSeedType == SeedType::SEED_DOOMSHROOM || mSeedType == SeedType::SEED_ICESHROOM ||
            mSeedType == SeedType::SEED_PICKLEDPEPPER)
        {
            DoSpecial();
            return;
        }
        else if (mSeedType == SeedType::SEED_POTATOMINE && mState != PlantState::STATE_NOTREADY)
        {
            DoSpecial();
            return;
        }
    }

    if (mSeedType == SeedType::SEED_SQUASH && mState != PlantState::STATE_NOTREADY)
        return;

    mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_GRAVE_STONE, mRow, 8);
    mSquished = true;
    mDisappearCountdown = 500;
    mApp->PlayFoley(FoleyType::FOLEY_SQUISH);
    RemoveEffects();

    GridItem* aLadder = mBoard->GetLadderAt(mPlantCol, mRow);
    if (aLadder)
    {
        aLadder->GridItemDie();
    }

    if (mApp->IsIZombieLevel())
    {
        mBoard->mChallenge->IZombiePlantDropRemainingSun(this);
    }
}

void Plant::UpdateBowling()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim && aBodyReanim->TrackExists("_ground"))
    {
        float aSpeed = aBodyReanim->GetTrackVelocity("_ground");
        if (mSeedType == SeedType::SEED_GIANT_WALLNUT)
        {
            aSpeed *= 2;
        }

        mX -= aSpeed;
        if (mX > 800)
            Die();
    }

    if (mState == PlantState::STATE_BOWLING_UP)
    {
        mY -= 2;
    }
    else if (mState == PlantState::STATE_BOWLING_DOWN)
    {
        mY += 2;
    }
    int aDistToGrid = mBoard->GridToPixelY(0, mRow) - mY;
    if (aDistToGrid < -2 || aDistToGrid > 2)
        return;

    PlantState aNewState = mState;
    if (mState == PlantState::STATE_BOWLING_UP && mRow <= 0)
    {
        aNewState = PlantState::STATE_BOWLING_DOWN;
    }
    else if (mState == PlantState::STATE_BOWLING_DOWN && mRow >= 4)
    {
        aNewState = PlantState::STATE_BOWLING_UP;
    }

    Zombie* aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
    if (aZombie)
    {
        int aPosX = mX + mWidth / 2;
        int aPosY = mY + mHeight / 2;

        if (mSeedType == SeedType::SEED_EXPLODE_O_NUT)
        {
            mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB);
            mApp->PlaySample(SOUND_BOWLINGIMPACT2);

            int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY) | 32U;
            mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 90, 1, true, aDamageRangeFlags);
            mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POWIE);
            mBoard->ShakeBoard(3, -4);

            Die();

            return;
        }

        mApp->PlayFoley(FoleyType::FOLEY_BOWLINGIMPACT);
        mBoard->ShakeBoard(1, -2);

        if (mSeedType == SeedType::SEED_GIANT_WALLNUT)
        {
            aZombie->TakeDamage(1800, 0U);
        }
        else if (aZombie->mShieldType == ShieldType::SHIELDTYPE_DOOR && mState != PlantState::STATE_NOTREADY)
        {
            aZombie->TakeDamage(1800, 0U);
        }
        else if (aZombie->mShieldType != ShieldType::SHIELDTYPE_NONE)
        {
            aZombie->TakeShieldDamage(400, 0U);
        }
        else if (aZombie->mHelmType != HelmType::HELMTYPE_NONE)
        {
            if (aZombie->mHelmType == HelmType::HELMTYPE_PAIL)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SHIELD_HIT);
            }
            else if (aZombie->mHelmType == HelmType::HELMTYPE_TRAFFIC_CONE)
            {
                mApp->PlayFoley(FoleyType::FOLEY_PLASTIC_HIT);
            }
            
            aZombie->TakeHelmDamage(900, 0U);
        }
        else
        {
            aZombie->TakeDamage(1800, 0U);
        }

        if ((!mApp->IsFirstTimeAdventureMode() || mApp->mPlayerInfo->mLevel > 10) && mSeedType == SeedType::SEED_WALLNUT && !mApp->mPlayedQuickplay)
        {
            mLaunchCounter++;
            if (mLaunchCounter == 2)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
                mBoard->AddCoin(aPosX, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
            }
            else if (mLaunchCounter == 3)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
                mBoard->AddCoin(aPosX - 5.0f, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
                mBoard->AddCoin(aPosX + 5.0f, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
            }
            else if (mLaunchCounter == 4)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
                mBoard->AddCoin(aPosX - 10.0f, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
                mBoard->AddCoin(aPosX, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
                mBoard->AddCoin(aPosX + 10.0f, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
            }
            else if (mLaunchCounter >= 5)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
                mBoard->AddCoin(aPosX, aPosY, CoinType::COIN_GOLD, CoinMotion::COIN_MOTION_COIN);
                mApp->GetAchievement(ACHIEVEMENT_ROLL_SOME_HEADS);
            }
        }

        if (mSeedType != SeedType::SEED_GIANT_WALLNUT)
        {
            if (mRow == 4 || mState == PlantState::STATE_BOWLING_DOWN)
            {
                aNewState = PlantState::STATE_BOWLING_UP;
            }
            else if (mRow == 0 || mState == PlantState::STATE_BOWLING_UP)
            {
                aNewState = PlantState::STATE_BOWLING_DOWN;
            }
            else
            {
                aNewState = Sexy::Rand(2) ? PlantState::STATE_BOWLING_UP : PlantState::STATE_BOWLING_DOWN;
            }
        }
    }

    if (aNewState == PlantState::STATE_BOWLING_UP)
    {
        mRow--;
        mState = PlantState::STATE_BOWLING_UP;
        mRenderOrder = CalcRenderOrder();
    }
    else if (aNewState == PlantState::STATE_BOWLING_DOWN)
    {
        mState = PlantState::STATE_BOWLING_DOWN;
        mRenderOrder = CalcRenderOrder();
        mRow++;
    }
}

void Plant::UpdateAbilities()
{
    if (mSeedType == SeedType::SEED_PLANTERN && !mIsAsleep && mBoard)
    {
        if (mParticleID == PARTICLEID_NULL)
        {
            TodParticleSystem* aParticle = mApp->AddTodParticle(mX + 40, mY + 40, (int)RenderLayer::RENDER_LAYER_FOG + 1, ParticleEffect::PARTICLE_LANTERN_SHINE);
            if (aParticle)
            {
                mParticleID = mApp->ParticleGetID(aParticle);
            }
        }
        UpdatePlanternEffects(1.5f, 30);
    }
    else if (mSeedType == SeedType::SEED_ICE_PLANTERN && !mIsAsleep && mBoard)
    {
        if (mParticleID == PARTICLEID_NULL)
        {
            TodParticleSystem* aParticle = mApp->AddTodParticle(mX + 40, mY + 40, (int)RenderLayer::RENDER_LAYER_FOG + 1, ParticleEffect::PARTICLE_LANTERN_SHINE);
            if (aParticle)
            {
                mParticleID = mApp->ParticleGetID(aParticle);
            }
        }
        UpdatePlanternEffects(2.0f, 60);

        // Ice Plantern's unique chilling ability
        if (mIcePlanternChillCounter > 0)
        {
            mIcePlanternChillCounter--;
        }
        else
        {
            mIcePlanternChillCounter = 200; // Reset the counter (2-second interval)
            mApp->PlayFoley(FOLEY_SNOW_PEA_SPARKLES);

            Rect aChillRect = GetPlantRect();
            aChillRect.mX -= 160;
            aChillRect.mWidth += 320;
            aChillRect.mY -= 160;
            aChillRect.mHeight += 320;

            Zombie* aZombie = nullptr;
            while (mBoard->IterateZombies(aZombie))
            {
                if (!aZombie->IsDeadOrDying() && aZombie->GetZombieRect().mX < 800)
                {
                    Rect aZombieRect = aZombie->GetZombieRect();
                    if (aChillRect.Intersects(aZombieRect))
                    {
                        aZombie->TakeDamage(20, 0); // Deal 20 damage
                        if (Rand(10) == 0) // 10% chance to freeze
                        {
                            aZombie->HitIceTrap();
                        }
                        else
                        {
                            aZombie->ApplyChill(false);
                        }
                    }
                }
            }
        }
    }

    if (!IsInPlay())
        return;

    if (mState == PlantState::STATE_DOINGSPECIAL || mSquished)
    {
        mDisappearCountdown--;
        if (mDisappearCountdown < 0)
        {
            Die();
            return;
        }
    }

    if (mWakeUpCounter > 0)
    {
        mWakeUpCounter--;
        if (mWakeUpCounter == 60)
        {
            mApp->PlayFoley(FoleyType::FOLEY_WAKEUP);
        }
        if (mWakeUpCounter == 0)
        {
            SetSleeping(false);
        }
    }

    if (mIsAsleep || mSquished || mOnBungeeState != PlantOnBungeeState::NOT_ON_BUNGEE)
        return;
    
    UpdateShooting();

    if (mStateCountdown > 0)
        mStateCountdown--;

    if (mApp->IsWallnutBowlingLevel())
    {
        UpdateBowling();
        return;
    }

    if (mSeedType == SeedType::SEED_SQUASH)                                                     UpdateSquash();
    else if (mSeedType == SeedType::SEED_DOOMSHROOM)                                            UpdateDoomShroom();
    else if (mSeedType == SeedType::SEED_ICESHROOM)                                             UpdateIceShroom();
    else if (mSeedType == SeedType::SEED_CHOMPER)                                               UpdateChomper();
    else if (mSeedType == SeedType::SEED_BLOVER)                                                UpdateBlover();
    else if (mSeedType == SeedType::SEED_FLOWERPOT)                                             UpdateFlowerPot();
    else if (mSeedType == SeedType::SEED_LILYPAD)                                               UpdateLilypad();
    else if (mSeedType == SeedType::SEED_IMITATER)                                              UpdateImitater();
    else if (mSeedType == SeedType::SEED_INSTANT_COFFEE)                                        UpdateCoffeeBean();
    else if (mSeedType == SeedType::SEED_UMBRELLA)                                              UpdateUmbrella();
    else if (mSeedType == SeedType::SEED_COBCANNON)                                             UpdateCobCannon();
    else if (mSeedType == SeedType::SEED_CACTUS || mSeedType == SeedType::SEED_MAD_CACTUS)      UpdateCactus();
    else if (mSeedType == SeedType::SEED_MAGNETSHROOM)                                          UpdateMagnetShroom();
    else if (mSeedType == SeedType::SEED_GOLD_MAGNET)                                           UpdateGoldMagnetShroom();
    else if (mSeedType == SeedType::SEED_SUNSHROOM)                                             UpdateSunShroom();
    else if (MakesSun())                                                                        UpdateProductionPlant();
    else if (mSeedType == SeedType::SEED_GRAVEBUSTER)                                           UpdateGraveBuster();
    else if (mSeedType == SeedType::SEED_TORCHWOOD || mSeedType == SeedType::SEED_BLUE_TORCHWOOD) UpdateTorchwood();
    else if (mSeedType == SeedType::SEED_POTATOMINE || mSeedType == SeedType::SEED_RED_POTATO_MINE) UpdatePotato();
    else if (mSeedType == SeedType::SEED_SPIKEWEED || mSeedType == SeedType::SEED_SPIKEROCK)    UpdateSpikeweed();
    else if (mSeedType == SeedType::SEED_TANGLEKELP)                                            UpdateTanglekelp();
    else if (mSeedType == SeedType::SEED_SCAREDYSHROOM)
    {
        if (mPlantCol >= 6)
        {
            if (!mIsAsleep)
            {
                if (mPlantMaxHealth != 1000)
                {
                    mPlantMaxHealth = 1000;
                    mPlantHealth = 1000;
                }
            }
            else
            {
                if (mPlantMaxHealth != 300)
                {
                    mPlantMaxHealth = 300;
                    if (mPlantHealth > 300) mPlantHealth = 300;
                }
            }
        }
        UpdateScaredyShroom();
    }
    else if (mSeedType == SeedType::SEED_SUPER_CHOMPER)                                         UpdateSuperChomper();
    else if (mSeedType == SeedType::SEED_BOMBSQUASH)                                            UpdateBombSquash();
    else if (mSeedType == SeedType::SEED_DOOM_NUT)                                              UpdateDoomnut();
    else if (mSeedType == SeedType::SEED_ABSOLUTELEAF)                                          UpdateAbsoluteLeaf();
    else if (mSeedType == SeedType::SEED_MARIGOLD)                                              UpdateMarigold();
    else if (mSeedType == SeedType::SEED_SPROUT)                                                UpdateSprout();
    else if (mSeedType == SeedType::SEED_COMMANDOPEA)                                           UpdateCommandoPea();
    else if (mSeedType == SeedType::SEED_SHRINKING_VIOLET)                                      UpdateShrinkingViolet();
    else if (mSeedType == SeedType::SEED_HATTREMWITCH)                                          UpdateHattremWitch();
    else if (mSeedType == SeedType::SEED_NIGHTCAP)                                              UpdateNightcap();
    else if (mSeedType == SeedType::SEED_SWEETPOTATO)                                           UpdateSweetPotato();
    else if (mSeedType == SeedType::SEED_GENERALPEA)                                            UpdateGeneralPea();
    else if (mSeedType == SeedType::SEED_SWEETEST_POTATO)                                       UpdateSweetestPotato();
    else if (mSeedType == SeedType::SEED_HATTREMSAGE)                                           UpdateHattremSage();
    else if (mSeedType == SeedType::SEED_DARKCAP)                                               UpdateDarkcap();

    if (mSubclass == PlantSubClass::SUBCLASS_SHOOTER && mSeedType != SeedType::SEED_COMMANDOPEA && mSeedType != SeedType::SEED_GENERALPEA && mSeedType != SeedType::SEED_NIGHTCAP && mSeedType != SeedType::SEED_DARKCAP)
    {
        UpdateShooter();
    }
    if (mDoSpecialCountdown > 0)
    {
        mDoSpecialCountdown--;
        if (mDoSpecialCountdown == 0)
        {
            DoSpecial();
        }
    }
}

bool Plant::IsPartOfUpgradableTo(SeedType theUpgradedType)
{
    /*
    if (theUpgradedType == SeedType::SEED_COBCANNON && mSeedType == SeedType::SEED_KERNELPULT)
    {
        return mBoard->IsValidCobCannonSpot(mPlantCol, mRow) || mBoard->IsValidCobCannonSpot(mPlantCol - 1, mRow);
    }
    */

    return IsUpgradableTo(theUpgradedType);
}

bool Plant::IsUpgradableTo(SeedType theUpgradedType)
{
    /* unused
    if (theUpgradedType == SeedType::SEED_GATLINGPEA && mSeedType == SeedType::SEED_REPEATER)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_WINTERMELON && mSeedType == SeedType::SEED_MELONPULT)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_TWINSUNFLOWER && mSeedType == SeedType::SEED_SUNFLOWER)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_SPIKEROCK && mSeedType == SeedType::SEED_SPIKEWEED)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_COBCANNON && mSeedType == SeedType::SEED_KERNELPULT)
    {
        return mBoard->IsValidCobCannonSpot(mPlantCol, mRow);
    }
    if (theUpgradedType == SeedType::SEED_GOLD_MAGNET && mSeedType == SeedType::SEED_MAGNETSHROOM)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_GLOOMSHROOM && mSeedType == SeedType::SEED_FUMESHROOM)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_CATTAIL && mSeedType == SeedType::SEED_LILYPAD)
    {
        Plant* aPlant = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
        return aPlant == nullptr || aPlant->mSeedType != SeedType::SEED_CATTAIL;
    }
    */
    return false;
}

bool Plant::IsUpgradable(SeedType theSeedType)
{
    switch (theSeedType)
    {
    case SeedType::SEED_SUNFLOWER:
    case SeedType::SEED_REPEATER:
    case SeedType::SEED_FUMESHROOM:
    case SeedType::SEED_LILYPAD:
    case SeedType::SEED_SPIKEWEED:
    case SeedType::SEED_MELONPULT:
    case SeedType::SEED_MAGNETSHROOM:
    case SeedType::SEED_KERNELPULT:
    case SeedType::SEED_PEASHOOTER:
    case SeedType::SEED_WALLNUT:
    case SeedType::SEED_POTATOMINE:
    case SeedType::SEED_SNOWPEA:
    case SeedType::SEED_CHOMPER:
    case SeedType::SEED_HYPNOSHROOM:
    case SeedType::SEED_SQUASH:
    case SeedType::SEED_THREEPEATER:
    case SeedType::SEED_TORCHWOOD:
    case SeedType::SEED_TALLNUT:
    case SeedType::SEED_PLANTERN:
    case SeedType::SEED_CACTUS:
    case SeedType::SEED_STARFRUIT:
    case SeedType::SEED_CABBAGEPULT:
    case SeedType::SEED_GARLIC:
    case SeedType::SEED_UMBRELLA:
    case SeedType::SEED_SUNBEAN:
    case SeedType::SEED_COMMANDOPEA:
    case SeedType::SEED_SWEETPOTATO:
    case SeedType::SEED_HATTREMWITCH:
    case SeedType::SEED_NIGHTCAP:
    case SeedType::SEED_SPLITPEA:
        return true;

    default:
        return false;
    }
}

void Plant::UpdateReanimColor()
{
    if (!IsOnBoard())
        return;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    // Start with a default white color
    Color aColorOverride = Color::White;

    // 1. Set the base color based on the Gloom-shroom's current mode
    if (mSeedType == SEED_GLOOMSHROOM)
    {
        if (mGloomMode == GLOOM_RAPID)
            aColorOverride = Color(255, 128, 128); // Red tint
        else if (mGloomMode == GLOOM_RANGER)
            aColorOverride = Color(128, 255, 128); // Green tint
        else if (mGloomMode == GLOOM_RAPID_RANGER)
            aColorOverride = Color(100, 100, 100); // Dark tint
    }
    else if (mSeedType == SeedType::SEED_SUPER_SPLITPEA)
    {
        int flashVal = mSpecialAttackCount * 25;
        aColorOverride = Color(255, 120 + flashVal, 255);
        Reanimation* aHead1 = mApp->ReanimationTryToGet(mHeadReanimID);
        Reanimation* aHead2 = mApp->ReanimationTryToGet(mHeadReanimID2);
        if (aHead1) aHead1->mColorOverride = aColorOverride;
        if (aHead2) aHead2->mColorOverride = aColorOverride;
    }

    // 2. Allow other states, like being targeted for an upgrade, to temporarily overwrite the base color
    SeedType aSeedType = mBoard->GetSeedTypeInCursor();
    if (IsPartOfUpgradableTo(aSeedType) && mBoard->CanPlantAt(mPlantCol, mRow, aSeedType) == PLANTING_OK)
    {
        aColorOverride = GetFlashingColor(mBoard->mMainCounter, 90);
    }
    else if (mSeedType == SeedType::SEED_SUNSHROOM && aSeedType == SeedType::SEED_SUNSHROOM && mBoard->CanPlantAt(mPlantCol, mRow, SeedType::SEED_SUNSHROOM) == PlantingReason::PLANTING_OK)
    {
        aColorOverride = GetFlashingColor(mBoard->mMainCounter, 90);
    }
    else if (mSeedType == SeedType::SEED_SCAREDYSHROOM && mBoard && mBoard->mApp->IsAdventureMode() && mBoard->mLevel == 17 && !mBoard->mScaredyShroomTutorialCompleted && this == mBoard->GetFirstPlantByType(SeedType::SEED_SCAREDYSHROOM))
    {
        aColorOverride = GetFlashingColor(mBoard->mMainCounter, 90);
    }
    else if (mBoard->mCursorObject->mCursorType == CURSOR_TYPE_FERTILIZER && IsUpgradable(mSeedType))
    {
        aColorOverride = GetFlashingColor(mBoard->mMainCounter, 90);
    }

    // Apply the final calculated base color
    aBodyReanim->mColorOverride = aColorOverride;

    // 3. Handle high-priority additive effects (glows and flashes) that appear on top
    if (mHighlighted)
    {
        aBodyReanim->mExtraAdditiveColor = Color(255, 255, 255, 196);
        aBodyReanim->mEnableExtraAdditiveDraw = true;
    }
    else if (mChilledCounter > 0)
    {
        // The blue "chilled" effect is the highest priority and overrides the base color
        aBodyReanim->mColorOverride = Color(75, 75, 255, 255);
        aBodyReanim->mExtraAdditiveColor = Color(75, 75, 255, 255);
        aBodyReanim->mEnableExtraAdditiveDraw = true;
    }
    else if (mBeghouledFlashCountdown > 0)
    {
        int anAlpha = TodAnimateCurve(50, 0, mBeghouledFlashCountdown % 50, 0, 128, TodCurves::CURVE_BOUNCE);
        aBodyReanim->mExtraAdditiveColor = Color(255, 255, 255, anAlpha);
        aBodyReanim->mEnableExtraAdditiveDraw = true;
    }
    else if (mEatenFlashCountdown > 0)
    {
        int aGrayness = ClampInt(mEatenFlashCountdown * 3, 0, mImitaterType == SEED_IMITATER ? 128 : 255);
        aBodyReanim->mExtraAdditiveColor = Color(aGrayness, aGrayness, aGrayness);
        aBodyReanim->mEnableExtraAdditiveDraw = true;
    }
    else
    {
        aBodyReanim->mEnableExtraAdditiveDraw = false;
    }

    aBodyReanim->PropogateColorToAttachments();
}

bool Plant::IsOnBoard()
{
    if (!mIsOnBoard)
        return false;

    TOD_ASSERT(mBoard);
    return true;
}

bool Plant::IsInPlay()
{
    return IsOnBoard() && mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN && mApp->mGameMode != GameMode::GAMEMODE_TREE_OF_WISDOM;
}

void Plant::UpdateReanim()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    UpdateReanimColor();

    if (mChilledCounter > 0)
    {
        aBodyReanim->mAnimRate = 1.2f;
    }

    if (mIsBoosted)
    {
        aBodyReanim->Update();
        aBodyReanim->Update();
    }

    float aOffsetX = mShakeOffsetX;
    float aOffsetY = PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);
    float aScaleX = 1.0f, aScaleY = 1.0f;
    if ((mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME) &&
        (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_SUNFLOWER || mSeedType == SeedType::SEED_MARIGOLD))
    {
        aScaleX = 1.5f;
        aScaleY = 1.5f;
        aOffsetX -= 20.0f;
        aOffsetY -= 40.0f;
    }

    if (mSeedType == SeedType::SEED_NIGHTCAP && mIsHiding)
    {
        aBodyReanim->mColorOverride = Color(128, 128, 128);
    }

    if (mSeedType == SeedType::SEED_DARKCAP && mIsHiding)
    {
        aBodyReanim->mColorOverride = Color(100, 100, 100);
    }

    if (mSeedType == SeedType::SEED_GIANT_WALLNUT)
    {
        aScaleX = 2.0f;
        aScaleY = 2.0f;
        aOffsetX -= 76.0f;
        aOffsetY -= 64.0f;
    }
    if (mSeedType == SeedType::SEED_INSTANT_COFFEE)
    {
        aScaleX = 0.8f;
        aScaleY = 0.8f;
        aOffsetX += 12.0f;
        aOffsetY += 10.0f;
    }
    if (mSeedType == SeedType::SEED_POTATOMINE || mSeedType == SeedType::SEED_RED_POTATO_MINE)
    {
        aScaleX = 0.8f;
        aScaleY = 0.8f;
        aOffsetX += 12.0f;
        aOffsetY += 12.0f;
    }
    if (mState == PlantState::STATE_GRAVEBUSTER_EATING)
    {
        aOffsetY += TodAnimateCurveFloat(400, 0, mStateCountdown, 0.0f, 30.0f, TodCurves::CURVE_LINEAR);
    }
    if (mWakeUpCounter > 0)
    {
        float aScaleFactor = TodAnimateCurveFloat(70, 0, mWakeUpCounter, 1.0f, 0.8f, TodCurves::CURVE_EASE_SIN_WAVE);
        aScaleY *= aScaleFactor;
        aOffsetY += 80.0f - 80.0f * aScaleFactor;
    }

    aBodyReanim->Update();

    if (mSeedType == SeedType::SEED_LEFTPEATER)
    {
        aOffsetX += 80.0f * aScaleX;
        aScaleX *= -1.0f;
    }

    if (mPottedPlantIndex != -1)
    {
        PottedPlant* aPottedPlant = &mApp->mPlayerInfo->mPottedPlant[mPottedPlantIndex];

        if (aPottedPlant->mFacing == PottedPlant::FacingDirection::FACING_LEFT)
        {
            aOffsetX += 80.0f * aScaleX;
            aScaleX *= -1.0f;
        }

        float aOffsetXStart, aOffsetXEnd;
        float aOffsetYStart, aOffsetYEnd;
        float aScaleStart, aScaleEnd;
        if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_SMALL)
        {
            aOffsetXStart = 20.0f;
            aOffsetXEnd = 20.0f;
            aOffsetYStart = 40.0f;
            aOffsetYEnd = 40.0f;
            aScaleStart = 0.5f;
            aScaleEnd = 0.5f;
        }
        else if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_MEDIUM)
        {
            aOffsetXStart = 20.0f;
            aOffsetXEnd = 10.0f;
            aOffsetYStart = 40.0f;
            aOffsetYEnd = 20.0f;
            aScaleStart = 0.5f;
            aScaleEnd = 0.75f;
        }
        else
        {
            aOffsetXStart = 10.0f;
            aOffsetXEnd = 0.0f;
            aOffsetYStart = 20.0f;
            aOffsetYEnd = 0.0f;
            aScaleStart = 0.75f;
            aScaleEnd = 1.0f;
        }

        float aAnimatedOffsetX = TodAnimateCurveFloat(100, 0, mStateCountdown, aOffsetXStart, aOffsetXEnd, TodCurves::CURVE_LINEAR);
        float aAnimatedOffsetY = TodAnimateCurveFloat(100, 0, mStateCountdown, aOffsetYStart, aOffsetYEnd, TodCurves::CURVE_LINEAR);
        float aAnimatedScale = TodAnimateCurveFloat(100, 0, mStateCountdown, aScaleStart, aScaleEnd, TodCurves::CURVE_LINEAR);

        aOffsetX += aAnimatedOffsetX * aScaleX;
        aOffsetY += aAnimatedOffsetY * aScaleY;
        aScaleX *= aAnimatedScale;
        aScaleY *= aAnimatedScale;
        aOffsetX += mApp->mZenGarden->ZenPlantOffsetX(aPottedPlant);
        aOffsetY += mApp->mZenGarden->PlantPottedDrawHeightOffset(mSeedType, aScaleY);
    }

    aBodyReanim->SetPosition(aOffsetX, aOffsetY);
    aBodyReanim->OverrideScale(aScaleX, aScaleY);
}

void Plant::Update()
{
    SeedType aEffectiveSeed = (mSeedType == SeedType::SEED_IMITATER) ? mImitaterType : mSeedType;
    bool isInstantExplosive = (aEffectiveSeed == SeedType::SEED_JALAPENO || 
                               aEffectiveSeed == SeedType::SEED_CHERRYBOMB || 
                               aEffectiveSeed == SeedType::SEED_DOOMSHROOM || 
                               aEffectiveSeed == SeedType::SEED_ICESHROOM || 
                               aEffectiveSeed == SeedType::SEED_PICKLEDPEPPER);

    if (mFrozenCounter > 0 && !isInstantExplosive)
    {
        mFrozenCounter--;
        // If just thawed, resume normal animation
        if (mFrozenCounter == 0 && mDoSpecialCountdown == 0 && mState != PlantState::STATE_DOINGSPECIAL)
        {
            PlayIdleAnim(mBodyReanimID != REANIMATIONID_NULL ? mApp->ReanimationGet(mBodyReanimID)->mDefinition->mFPS : 0.0f);
        }
        return;
    }

    if (mButteredCounter > 0 && !isInstantExplosive)
    {
        mButteredCounter--;
        return; // Skip the rest of the update logic while buttered
    }

    if (mChilledCounter > 0 && !isInstantExplosive)
    {
        mChilledCounter--;
        if (mChilledCounter == 0 && mDoSpecialCountdown == 0 && mState != PlantState::STATE_DOINGSPECIAL)
        {
            PlayIdleAnim(mBodyReanimID != REANIMATIONID_NULL ? mApp->ReanimationGet(mBodyReanimID)->mDefinition->mFPS : 0.0f);
        }
    }

    if (mBoostCounter > 0)
    {
        mBoostCounter--;
        if (mBoostCounter == 0)
        {
            mIsBoosted = false;
        }
    }

    if (mSeedType == SEED_SCAREDYSHROOM)
    {
        // If in random mode and the filter is not fully visible, fade it in
        if (mIsRandom && mGreenFilterEffect < 150)
        {
            mGreenFilterEffect = min(mGreenFilterEffect + 10, 150);
        }
        // If not in random mode and the filter is visible, fade it out
        else if (!mIsRandom && mGreenFilterEffect > 0)
        {
            mGreenFilterEffect = max(mGreenFilterEffect - 10, 0);
        }
    }

    bool doUpdate = false;
    if (IsOnBoard() && mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO && mApp->IsWallnutBowlingLevel())
        doUpdate = true;
    else if (IsOnBoard() && mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
        doUpdate = true;
    else if (IsOnBoard() && mBoard->mCutScene->ShouldRunUpsellBoard())
        doUpdate = true;
    else if (!IsOnBoard() || mApp->mGameScene == GameScenes::SCENE_PLAYING)
        doUpdate = true;

    if (doUpdate)
    {
        UpdateAbilities();
        Animate();

        if (mPlantHealth < 0)
            Die();

        UpdateReanim();
    }
}

bool Plant::NotOnGround()
{
    if (mSeedType == SeedType::SEED_SQUASH || mSeedType == SeedType::SEED_BOMBSQUASH)
    {
        if (mState == PlantState::STATE_SQUASH_RISING || mState == PlantState::STATE_SQUASH_FALLING || mState == PlantState::STATE_SQUASH_DONE_FALLING)
            return true;
    }

    return mSquished || mOnBungeeState == PlantOnBungeeState::RISING_WITH_BUNGEE || mDead;
}

Reanimation* Plant::AttachBlinkAnim(Reanimation* theReanimBody)
{
    const PlantDefinition& aPlantDef = GetPlantDefinition(mSeedType);
    LawnApp* aApp = (LawnApp*)gSexyAppBase;
    Reanimation* aAnimToAttach = theReanimBody;
    const char* aTrackToPlay = "anim_blink";
    const char* aTrackToAttach = nullptr;

    if (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_TALLNUT || 
        mSeedType == SeedType::SEED_EXPLODE_O_NUT || mSeedType == SeedType::SEED_GIANT_WALLNUT ||
        mSeedType == SeedType::SEED_DOOM_NUT)
    {
        int aHit = Rand(10);
        if (aHit < 1 && theReanimBody->TrackExists("anim_blink_twitch"))
        {
            aTrackToPlay = "anim_blink_twitch";
        }
        else
        {
            aTrackToPlay = aHit < 7 ? "anim_blink_twice" : "anim_blink_thrice";
        }
    }
    else if (mSeedType == SeedType::SEED_THREEPEATER || mSeedType == SeedType::SEED_BLACK_THREEPEATER)
    {
        int aHit = Rand(3);
        if (aHit == 0)
        {
            aTrackToPlay = "anim_blink1";
            aTrackToAttach = "anim_face1";
            ReanimatorTrackInstance* aTrackInstance = theReanimBody->GetTrackInstanceByName("anim_head1");
            aAnimToAttach = FindReanimAttachment(aTrackInstance->mAttachmentID);
        }
        else if (aHit == 1)
        {
            aTrackToPlay = "anim_blink2";
            aTrackToAttach = "anim_face2";
            ReanimatorTrackInstance* aTrackInstance = theReanimBody->GetTrackInstanceByName("anim_head2");
            aAnimToAttach = FindReanimAttachment(aTrackInstance->mAttachmentID);
        }
        else
        {
            aTrackToPlay = "anim_blink3";
            aTrackToAttach = "anim_face3";
            ReanimatorTrackInstance* aTrackInstance = theReanimBody->GetTrackInstanceByName("anim_head3");
            aAnimToAttach = FindReanimAttachment(aTrackInstance->mAttachmentID);
        }
    }
    else if (mSeedType == SeedType::SEED_SPLITPEA || mSeedType == SeedType::SEED_SUPER_SPLITPEA)
    {
        if (Rand(2) == 0)
        {
            aTrackToPlay = "anim_blink";
            aTrackToAttach = "anim_face";
            aAnimToAttach = mApp->ReanimationTryToGet(mHeadReanimID);
        }
        else
        {
            aTrackToPlay = "anim_blink2";
            aTrackToAttach = "anim_face2";
            aAnimToAttach = mApp->ReanimationTryToGet(mHeadReanimID2);
        }
    }
    else if (mSeedType == SeedType::SEED_TWINSUNFLOWER)
    {
        if (Rand(2) == 0)
        {
            aTrackToPlay = "anim_blink";
            aTrackToAttach = "anim_face";
        }
        else
        {
            aTrackToPlay = "anim_blink2";
            aTrackToAttach = "anim_face2";
        }
    }
    else if (mSeedType == SeedType::SEED_PEASHOOTER || mSeedType == SeedType::SEED_SNOWPEA || mSeedType == SeedType::SEED_REPEATER || 
        mSeedType == SeedType::SEED_LEFTPEATER || mSeedType == SeedType::SEED_GATLINGPEA || mSeedType == SeedType::SEED_ICEPEA ||
        mSeedType == SeedType::SEED_FIRESHOOTER || mSeedType == SeedType::SEED_COMMANDOPEA || mSeedType == SeedType::SEED_GENERALPEA)
    {
        if (theReanimBody->TrackExists("anim_stem"))
        {
            ReanimatorTrackInstance* aTrackInstance = theReanimBody->GetTrackInstanceByName("anim_stem");
            aAnimToAttach = FindReanimAttachment(aTrackInstance->mAttachmentID);
        }
        else if (theReanimBody->TrackExists("anim_idle"))
        {
            ReanimatorTrackInstance* aTrackInstance = theReanimBody->GetTrackInstanceByName("anim_idle");
            aAnimToAttach = FindReanimAttachment(aTrackInstance->mAttachmentID);
        }
    }

    if (aAnimToAttach == nullptr)
    {
        TodTrace("Missing head anim");
        return nullptr;
    }

    if (!theReanimBody->TrackExists(aTrackToPlay))
        return nullptr;

    Reanimation* aBlinkReanim = aApp->mEffectSystem->mReanimationHolder->AllocReanimation(0.0f, 0.0f, 0, aPlantDef.mReanimationType);
    aBlinkReanim->SetFramesForLayer(aTrackToPlay);
    aBlinkReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_FULL_LAST_FRAME_AND_HOLD;
    aBlinkReanim->mAnimRate = 15.0f;
    aBlinkReanim->mColorOverride = theReanimBody->mColorOverride;

    if (aTrackToAttach && aAnimToAttach->TrackExists(aTrackToAttach))
    {
        aBlinkReanim->AttachToAnotherReanimation(aAnimToAttach, aTrackToAttach);
    }
    else if (aAnimToAttach->TrackExists("anim_face"))
    {
        aBlinkReanim->AttachToAnotherReanimation(aAnimToAttach, "anim_face");
    }
    else if (aAnimToAttach->TrackExists("anim_idle"))
    {
        aBlinkReanim->AttachToAnotherReanimation(aAnimToAttach, "anim_idle");
    }
    else
    {
        TodTrace("Missing anim_idle for blink");
    }

    aBlinkReanim->mFilterEffect = theReanimBody->mFilterEffect;
    return aBlinkReanim;
}

void Plant::DoBlink()
{
    mBlinkCountdown = 400 + Rand(400);

    if (NotOnGround() || mShootingCounter != 0)
        return;

    if ((mSeedType == SeedType::SEED_POTATOMINE || mSeedType == SeedType::SEED_RED_POTATO_MINE) && mState != PlantState::STATE_POTATO_ARMED)
        return;

    if (mState == PlantState::STATE_CACTUS_RISING || mState == PlantState::STATE_CACTUS_HIGH || mState == PlantState::STATE_CACTUS_LOWERING ||
        mState == PlantState::STATE_MAGNETSHROOM_SUCKING || mState == PlantState::STATE_MAGNETSHROOM_CHARGING)
        return;

    EndBlink();
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    if ((mSeedType == SeedType::SEED_TALLNUT && aBodyReanim->GetImageOverride("anim_idle") == IMAGE_REANIM_TALLNUT_CRACKED2) || 
        (mSeedType == SeedType::SEED_DOOM_NUT && aBodyReanim->GetImageOverride("anim_idle") == IMAGE_REANIM_DOOM_NUT_CRACKED2) ||
        (mSeedType == SeedType::SEED_GARLIC && aBodyReanim->GetImageOverride("anim_face") == IMAGE_REANIM_GARLIC_BODY3) ||
        (mSeedType == SeedType::SEED_CORROSION_GARLIC && aBodyReanim->GetImageOverride("anim_face") == IMAGE_REANIM_CORROSION_GARLIC_BODY3))
        return;

    if (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_TALLNUT || 
        mSeedType == SeedType::SEED_EXPLODE_O_NUT || mSeedType == SeedType::SEED_GIANT_WALLNUT ||
        mSeedType == SeedType::SEED_DOOM_NUT)
    {
        mBlinkCountdown = 1000 + Rand(1000);
    }

    Reanimation* aBlinkReanim = AttachBlinkAnim(aBodyReanim);
    if (aBlinkReanim)
    {
        mBlinkReanimID = mApp->ReanimationGetID(aBlinkReanim);
    }
    aBodyReanim->AssignRenderGroupToPrefix("anim_eye", RENDER_GROUP_HIDDEN);
}

void Plant::EndBlink()
{
    if (mBlinkReanimID != ReanimationID::REANIMATIONID_NULL)
    {
        mApp->RemoveReanimation(mBlinkReanimID);
        mBlinkReanimID = ReanimationID::REANIMATIONID_NULL;

        Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
        if (aBodyReanim)
        {
            aBodyReanim->AssignRenderGroupToPrefix("anim_eye", RENDER_GROUP_NORMAL);
        }
    }
}

void Plant::UpdateBlink()
{
    if (mBlinkReanimID != ReanimationID::REANIMATIONID_NULL)
    {
        Reanimation* aBlinkReanim = mApp->ReanimationTryToGet(mBlinkReanimID);
        if (aBlinkReanim == nullptr || aBlinkReanim->mLoopCount > 0)
        {
            EndBlink();
        }
    }

    if (mIsAsleep)
        return;

    if (mBlinkCountdown > 0)
    {
        mBlinkCountdown--;
        if (mBlinkCountdown == 0)
        {
            DoBlink();
        }
    }
}

void Plant::AnimateNuts()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    Image* aCracked1;
    Image* aCracked2;
    const char* aTrackToOverride;
    if (mSeedType == SeedType::SEED_WALLNUT)
    {
        aCracked1 = IMAGE_REANIM_WALLNUT_CRACKED1;
        aCracked2 = IMAGE_REANIM_WALLNUT_CRACKED2;
        aTrackToOverride = "anim_face";
    }
    else if (mSeedType == SeedType::SEED_TALLNUT)
    {
        aCracked1 = IMAGE_REANIM_TALLNUT_CRACKED1;
        aCracked2 = IMAGE_REANIM_TALLNUT_CRACKED2;
        aTrackToOverride = "anim_idle";
    }
    else if (mSeedType == SeedType::SEED_EXPLODE_O_NUT)
    {
        aCracked1 = IMAGE_REANIM_EXPLODE_O_NUT_CRACKED1;
        aCracked2 = IMAGE_REANIM_EXPLODE_O_NUT_CRACKED2;
        aTrackToOverride = "anim_face";
    }
    else if (mSeedType == SeedType::SEED_DOOM_NUT)
    {
        aCracked1 = IMAGE_REANIM_DOOM_NUT_CRACKED1;
        aCracked2 = IMAGE_REANIM_DOOM_NUT_CRACKED2;
        aTrackToOverride = "anim_idle";
    }
    else return;

    int aPosX = mX + 40;
    int aPosY = mY + 10;
    if (mSeedType == SeedType::SEED_TALLNUT || mSeedType == SeedType::SEED_DOOM_NUT)
    {
        aPosY -= 32;
    }

    Image* aImageOverride = aBodyReanim->GetImageOverride(aTrackToOverride);
    if (mPlantHealth < mPlantMaxHealth / 3)
    {
        if (aImageOverride != aCracked2)
        {
            aBodyReanim->SetImageOverride(aTrackToOverride, aCracked2);
            mApp->AddTodParticle(aPosX, aPosY, mRenderOrder + 4, ParticleEffect::PARTICLE_WALLNUT_EAT_LARGE);
        }
    }
    else if (mPlantHealth < mPlantMaxHealth * 2 / 3)
    {
        if (aImageOverride != aCracked1)
        {
            aBodyReanim->SetImageOverride(aTrackToOverride, aCracked1);
            mApp->AddTodParticle(aPosX, aPosY, mRenderOrder + 4, ParticleEffect::PARTICLE_WALLNUT_EAT_LARGE);
        }
    }
    else
    {
        aBodyReanim->SetImageOverride(aTrackToOverride, nullptr);
    }

    if (IsInPlay() && !mApp->IsIZombieLevel())
    {
        if (mRecentlyEatenCountdown > 0 && mDoSpecialCountdown == 0 && mState != PlantState::STATE_DOINGSPECIAL)
        {
            aBodyReanim->mAnimRate = 0.1f;
            return;
        }

        if (aBodyReanim->mAnimRate < 1.0f && mOnBungeeState != PlantOnBungeeState::RISING_WITH_BUNGEE)
        {
            aBodyReanim->mAnimRate = RandRangeFloat(10.0f, 15.0f);
        }
    }
}

void Plant::AnimateGarlic()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    Image* aImageOverride = aBodyReanim->GetImageOverride("anim_face");

    if (mPlantHealth < mPlantMaxHealth / 3)
    {
        if (aImageOverride != IMAGE_REANIM_GARLIC_BODY3)
        {
            aBodyReanim->SetImageOverride("anim_face", IMAGE_REANIM_GARLIC_BODY3);
            aBodyReanim->AssignRenderGroupToPrefix("Garlic_stem", RENDER_GROUP_HIDDEN);
        }
    }
    else if (mPlantHealth < mPlantMaxHealth * 2 / 3)
    {
        if (aImageOverride != IMAGE_REANIM_GARLIC_BODY2)
        {
            aBodyReanim->SetImageOverride("anim_face", IMAGE_REANIM_GARLIC_BODY2);
        }
    }
    else
    {
        aBodyReanim->SetImageOverride("anim_face", nullptr);
    }
}

void Plant::AnimateCorrosionGarlic()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    Image* aImageOverride = aBodyReanim->GetImageOverride("anim_face");

    if (mPlantHealth < mPlantMaxHealth / 3)
    {
        if (aImageOverride != IMAGE_REANIM_CORROSION_GARLIC_BODY3)
        {
            aBodyReanim->SetImageOverride("anim_face", IMAGE_REANIM_CORROSION_GARLIC_BODY3);
            aBodyReanim->AssignRenderGroupToPrefix("Garlic_stem", RENDER_GROUP_HIDDEN);
        }
    }
    else if (mPlantHealth < mPlantMaxHealth * 2 / 3)
    {
        if (aImageOverride != IMAGE_REANIM_CORROSION_GARLIC_BODY2)
        {
            aBodyReanim->SetImageOverride("anim_face", IMAGE_REANIM_CORROSION_GARLIC_BODY2);
        }
    }
    else
    {
        aBodyReanim->SetImageOverride("anim_face", nullptr);
    }
}

void Plant::AnimatePumpkin()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    Image* aImageOverride = aBodyReanim->GetImageOverride("Pumpkin_front");

    if (mPlantHealth < mPlantMaxHealth / 3)
    {
        if (aImageOverride != IMAGE_REANIM_PUMPKIN_DAMAGE3)
            aBodyReanim->SetImageOverride("Pumpkin_front", IMAGE_REANIM_PUMPKIN_DAMAGE3);
    }
    else if (mPlantHealth < mPlantMaxHealth * 2 / 3)
    {
        if (aImageOverride != IMAGE_REANIM_PUMPKIN_DAMAGE1)
            aBodyReanim->SetImageOverride("Pumpkin_front", IMAGE_REANIM_PUMPKIN_DAMAGE1);
    }
    else
    {
        aBodyReanim->SetImageOverride("Pumpkin_front", nullptr);
    }
}

void Plant::UpdateShooting()
{
    if (NotOnGround() || mShootingCounter == 0)
        return;

    mShootingCounter--;

    if (mSeedType == SeedType::SEED_FUMESHROOM && mShootingCounter == 15)
    {
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
        AddAttachedParticle(mX + 85, mY + 31, aRenderPosition, ParticleEffect::PARTICLE_FUMECLOUD);
    }

    if (mSeedType == SeedType::SEED_GLOOMSHROOM)
    {
        if (mShootingCounter == 136 || mShootingCounter == 108 || mShootingCounter == 80 || mShootingCounter == 52)
        {
            int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
            if (mGloomMode == GLOOM_RANGER || mGloomMode == GLOOM_RAPID_RANGER)
            {
                AddAttachedParticle(mX + 40, mY + 40, aRenderPosition, ParticleEffect::PARTICLE_GLOOMCLOUD_RANGER);
            }
            else
            {
                AddAttachedParticle(mX + 40, mY + 40, aRenderPosition, ParticleEffect::PARTICLE_GLOOMCLOUD);
            }
        }
        if (mShootingCounter == 126 || mShootingCounter == 98 || mShootingCounter == 70 || mShootingCounter == 42)
        {
            Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
        }
    }
    else if (mSeedType == SeedType::SEED_GATLINGPEA)
    {
        if (mShootingCounter == 18 || mShootingCounter == 35 || mShootingCounter == 51 || mShootingCounter == 68)
        {
            Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
        }
    }
    else if (mSeedType == SeedType::SEED_NIGHTCAP)
    {
        if (mShootingCounter == 75 || mShootingCounter == 50 || mShootingCounter == 25)
        {
            // Define the attack area, same as in FindNightcapTarget() (3 tiles in front)
            Rect aAttackRect(mX, mY, 240, mHeight);
            bool aSoundPlayed = false;

            Zombie* aZombie = nullptr;
            while (mBoard->IterateZombies(aZombie))
            {
                // Check if the zombie is a valid target in the same row
                if (aZombie->mRow == mRow && !aZombie->IsDeadOrDying() && aZombie->EffectedByDamage(1U << DAMAGES_GROUND))
                {
                    // Check if the zombie is within the attack area
                    Rect aZombieRect = aZombie->GetZombieRect();
                    if (GetRectOverlap(aAttackRect, aZombieRect) > 0)
                    {
                        // Play sound and particle effect only once per hit wave
                        if (!aSoundPlayed)
                        {
                            mApp->PlayFoley(FOLEY_SPLAT);
                            if (mShootingCounter == 75) // Add effect on the first hit
                            {
                                mApp->AddTodParticle(aZombie->mX + 60, aZombie->mY + 60, aZombie->mRenderOrder + 1, PARTICLE_MIND_CONTROL);
                            }
                            aSoundPlayed = true;
                        }

                        // Damage the zombie
                        aZombie->TakeDamage(40, (1U << DAMAGE_BYPASSES_SHIELD));
                    }
                }
            }
        }

        if (mShootingCounter == 0)
        {
            mState = STATE_READY;
            mLaunchCounter = mLaunchRate;
            if (!mIsHiding && !mIsAsleep)
            {
                PlayBodyReanim("anim_idle", REANIM_LOOP, 10, 15.0f);
            }
        }
        return;
    }
    else if (mSeedType == SeedType::SEED_DARKCAP)
    {
        if (mShootingCounter == 100 || mShootingCounter == 75 || mShootingCounter == 50 || mShootingCounter == 25)
        {
            // Define the attack area, same as in FindNightcapTarget() (4 tiles in front)
            Rect aAttackRect(mX, mY, 320, mHeight);
            bool aSoundPlayed = false;

            Zombie* aZombie = nullptr;
            while (mBoard->IterateZombies(aZombie))
            {
                // Check if the zombie is a valid target in the same row
                if (aZombie->mRow == mRow && !aZombie->IsDeadOrDying() && aZombie->EffectedByDamage(1U << DAMAGES_GROUND))
                {
                    // Check if the zombie is within the attack area
                    Rect aZombieRect = aZombie->GetZombieRect();
                    if (GetRectOverlap(aAttackRect, aZombieRect) > 0)
                    {
                        // Play sound and particle effect only once per hit wave
                        if (!aSoundPlayed)
                        {
                            mApp->PlayFoley(FOLEY_SPLAT);
                            if (mShootingCounter == 100) // Add effect on the first hit
                            {
                                mApp->AddTodParticle(aZombie->mX + 60, aZombie->mY + 60, aZombie->mRenderOrder + 1, PARTICLE_JACKEXPLODE);
                            }
                            aSoundPlayed = true;
                        }

                        // Damage the zombie
                        aZombie->TakeDamage(80, (1U << DAMAGE_BYPASSES_SHIELD));
                    }
                }
            }
        }

        if (mShootingCounter == 0)
        {
            mState = STATE_READY;
            mLaunchCounter = mLaunchRate;
            if (!mIsHiding && !mIsAsleep)
            {
                PlayBodyReanim("anim_idle", REANIM_LOOP, 10, 15.0f);
            }
        }
        return;
    }
    else if (mSeedType == SeedType::SEED_CATTAIL)
    {
        if (mShootingCounter == 19)
        {
            Zombie* aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
            if (aZombie)
            {
                Fire(aZombie, mRow, PlantWeapon::WEAPON_PRIMARY);
            }
        }
    }
    else if (mShootingCounter == 1)
    {
        if (mSeedType == SeedType::SEED_THREEPEATER)
        {
            int rowAbove = mRow - 1;
            int rowBelow = mRow + 1;
            Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);
            Reanimation* aHeadReanim3 = mApp->ReanimationGet(mHeadReanimID3);
            Reanimation* aHeadReanim1 = mApp->ReanimationGet(mHeadReanimID);

            if (aHeadReanim1->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                Fire(nullptr, rowBelow, PlantWeapon::WEAPON_PRIMARY);
            }
            if (aHeadReanim2->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                int aBaseOriginX = mX + 45;
                int aBaseOriginY = mY + 10;

                Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);

                ProjectileType aSecondProjectileType;
                TodWeightedArray aProjectileArray[] = {
                    { (int)ProjectileType::PROJECTILE_PEA,          10 }, // Normal pea (40% chance)
                    { (int)ProjectileType::PROJECTILE_SNOWPEA,      10 }, // Snow pea (30% chance)
                    { (int)ProjectileType::PROJECTILE_RED_FIRE_PEA, 40 }, // Red Fire Pea (20% chance)
                    { (int)ProjectileType::PROJECTILE_BLUE_FIRE_PEA,40 }  // Blue Fire Pea (10% chance)
                };
                int aProjectileCount = sizeof(aProjectileArray) / sizeof(aProjectileArray[0]);
                aSecondProjectileType = (ProjectileType)TodPickFromWeightedArray(aProjectileArray, aProjectileCount);

                int aOriginX = mX + 45;
                int aOriginY = mY + 10;

                Projectile* aSecondProjectile = mBoard->AddProjectile(aBaseOriginX - 10, aBaseOriginY, mRenderOrder - 1, mRow, aSecondProjectileType);
                aSecondProjectile->mMotionType = ProjectileMotion::MOTION_STRAIGHT;
                aSecondProjectile->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
            }
            if (aHeadReanim3->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                Fire(nullptr, rowAbove, PlantWeapon::WEAPON_PRIMARY);
            }
        }
        else if (mSeedType == SeedType::SEED_BLACK_THREEPEATER)
        {
            int rowAbove = mRow - 1;
            int rowBelow = mRow + 1;
            Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);
            Reanimation* aHeadReanim3 = mApp->ReanimationGet(mHeadReanimID3);
            Reanimation* aHeadReanim1 = mApp->ReanimationGet(mHeadReanimID);

            if (aHeadReanim1->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                Fire(nullptr, rowBelow, PlantWeapon::WEAPON_PRIMARY);
            }
            if (aHeadReanim2->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                int aBaseOriginX = mX + 45;
                int aBaseOriginY = mY + 10;

                Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);

                ProjectileType aSecondProjectileType;
                TodWeightedArray aProjectileArray[] = {
                    { (int)ProjectileType::PROJECTILE_PEA,          10 }, // Normal pea (40% chance)
                    { (int)ProjectileType::PROJECTILE_SNOWPEA,      10 }, // Snow pea (30% chance)
                    { (int)ProjectileType::PROJECTILE_RED_FIRE_PEA, 40 }, // Red Fire Pea (20% chance)
                    { (int)ProjectileType::PROJECTILE_BLUE_FIRE_PEA,40 }  // Blue Fire Pea (10% chance)
                };
                int aProjectileCount = sizeof(aProjectileArray) / sizeof(aProjectileArray[0]);
                aSecondProjectileType = (ProjectileType)TodPickFromWeightedArray(aProjectileArray, aProjectileCount);

                int aOriginX = mX + 45;
                int aOriginY = mY + 10;

                Projectile* aSecondProjectile = mBoard->AddProjectile(aBaseOriginX - 10, aBaseOriginY, mRenderOrder - 1, mRow, aSecondProjectileType);
                aSecondProjectile->mMotionType = ProjectileMotion::MOTION_STRAIGHT;
                aSecondProjectile->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
            }
            if (aHeadReanim3->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                Fire(nullptr, rowAbove, PlantWeapon::WEAPON_PRIMARY);
            }
        }
        else if (mSeedType == SeedType::SEED_SPLITPEA)
        {
            Reanimation* aHeadBackReanim = mApp->ReanimationTryToGet(mHeadReanimID2);
            Reanimation* aHeadFrontReanim = mApp->ReanimationTryToGet(mHeadReanimID);
            if (aHeadFrontReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
                Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
            }
            if (aHeadBackReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                Fire(nullptr, mRow, PlantWeapon::WEAPON_SECONDARY);
                Fire(nullptr, mRow, PlantWeapon::WEAPON_SECONDARY);
            }
        }
        else if (mSeedType == SeedType::SEED_SUPER_SPLITPEA)
        {
            Reanimation* aHeadBackReanim = mApp->ReanimationTryToGet(mHeadReanimID2);
            Reanimation* aHeadFrontReanim = mApp->ReanimationTryToGet(mHeadReanimID);
            bool firedVolley = false;

            if (aHeadFrontReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
                Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
                Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
                firedVolley = true;
            }
            if (aHeadBackReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                Fire(nullptr, mRow, PlantWeapon::WEAPON_SECONDARY);
                Fire(nullptr, mRow, PlantWeapon::WEAPON_SECONDARY);
                Fire(nullptr, mRow, PlantWeapon::WEAPON_SECONDARY);
                firedVolley = true;
            }

            if (firedVolley)
            {
                mSpecialAttackCount++;
                if (mSpecialAttackCount >= 10)
                {
                    mSpecialAttackCount = 0;
                    TriggerSuperSplitPeaSpecial();
                }
            }
        }
        else if (mState == PlantState::STATE_CACTUS_LOW)
        {
            Fire(nullptr, mRow, PlantWeapon::WEAPON_SECONDARY);
        }
        else if (mSeedType == SeedType::SEED_CABBAGEPULT || mSeedType == SeedType::SEED_KERNELPULT || 
                mSeedType == SeedType::SEED_MELONPULT || mSeedType == SeedType::SEED_WINTERMELON || 
                mSeedType == SeedType::SEED_POISONPULT)
        {
            PlantWeapon aPlantWeapon = PlantWeapon::WEAPON_PRIMARY;
            if (mState == PlantState::STATE_KERNELPULT_BUTTER || mState == PlantState::STATE_KERNELPULT_BIG_BUTTER)
            {
                Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
                aBodyReanim->AssignRenderGroupToPrefix("Cornpult_butter", RENDER_GROUP_HIDDEN);
                aBodyReanim->AssignRenderGroupToPrefix("Cornpult_kernal", RENDER_GROUP_NORMAL);
                aPlantWeapon = PlantWeapon::WEAPON_SECONDARY;
            }

            Zombie* aZombie = FindTargetZombie(mRow, aPlantWeapon);
            Fire(aZombie, mRow, aPlantWeapon);

            if (mState == PlantState::STATE_KERNELPULT_BUTTER || mState == PlantState::STATE_KERNELPULT_BIG_BUTTER)
            {
                mState = PlantState::STATE_NOTREADY;
            }
            return;
        }
        else
        {
            Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
        }
    }

    if (mShootingCounter != 0)
        return;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mHeadReanimID);
    if (mSeedType == SeedType::SEED_THREEPEATER || mSeedType == SeedType::SEED_BLACK_THREEPEATER)
    {
        Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);
        Reanimation* aHeadReanim3 = mApp->ReanimationGet(mHeadReanimID3);

        if (aHeadReanim2->mLoopCount > 0)
        {
            if (aHeadReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                aHeadReanim->StartBlend(20);
                aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
                aHeadReanim->SetFramesForLayer("anim_head_idle1");
                aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
                aHeadReanim->mAnimTime = aBodyReanim->mAnimTime;
            }

            aHeadReanim2->StartBlend(20);
            aHeadReanim2->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim2->SetFramesForLayer("anim_head_idle2");
            aHeadReanim2->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim2->mAnimTime = aBodyReanim->mAnimTime;

            if (aHeadReanim3->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                aHeadReanim3->StartBlend(20);
                aHeadReanim3->mLoopType = ReanimLoopType::REANIM_LOOP;
                aHeadReanim3->SetFramesForLayer("anim_head_idle3");
                aHeadReanim3->mAnimRate = aBodyReanim->mAnimRate;
                aHeadReanim3->mAnimTime = aBodyReanim->mAnimTime;
            }
            
            return;
        }
    }
    else if (mSeedType == SeedType::SEED_SPLITPEA)
    {
        Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);

        if (aHeadReanim->mLoopCount > 0)
        {
            aHeadReanim->StartBlend(20);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->SetFramesForLayer("anim_head_idle");
            aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim->mAnimTime = aBodyReanim->mAnimTime;
        }

        if (aHeadReanim2->mLoopCount > 0)
        {
            aHeadReanim2->StartBlend(20);
            aHeadReanim2->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim2->SetFramesForLayer("anim_splitpea_idle");
            aHeadReanim2->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim2->mAnimTime = aBodyReanim->mAnimTime;
        }

        return;
    }
    else if (mState == PlantState::STATE_CACTUS_HIGH)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayBodyReanim("anim_idlehigh", ReanimLoopType::REANIM_LOOP, 20, 0.0f);

            aBodyReanim->mAnimRate = aBodyReanim->mDefinition->mFPS;
            if (mApp->IsIZombieLevel())
            {
                aBodyReanim->mAnimRate = 0.0f;
            }

            return;
        }
    }
    else if (aHeadReanim)
    {
        if (aHeadReanim->mLoopCount > 0)
        {
            aHeadReanim->StartBlend(20);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->SetFramesForLayer("anim_head_idle");
            aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim->mAnimTime = aBodyReanim->mAnimTime;
            return;
        }
    }
    else if (mSeedType == SeedType::SEED_COBCANNON)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_COBCANNON_ARMING;
            mStateCountdown = 3000;
            PlayBodyReanim("anim_unarmed_idle", ReanimLoopType::REANIM_LOOP, 20, aBodyReanim->mDefinition->mFPS);
            return;
        }
    }
    else if (aBodyReanim && aBodyReanim->mLoopCount > 0)
    {
        PlayIdleAnim(aBodyReanim->mDefinition->mFPS);
        return;
    }

    mShootingCounter = 1;
}

void Plant::Animate()
{
    if ((mSeedType == SeedType::SEED_CHERRYBOMB || mSeedType == SeedType::SEED_JALAPENO || mSeedType == SeedType::SEED_PICKLEDPEPPER) && mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
    {
        mShakeOffsetX = RandRangeFloat(-1.0f, 1.0f);
        mShakeOffsetY = RandRangeFloat(-1.0f, 1.0f);
    }

    if (mRecentlyEatenCountdown > 0)
    {
        mRecentlyEatenCountdown--;
    }
    if (mEatenFlashCountdown > 0)
    {
        mEatenFlashCountdown--;
    }
    if (mBeghouledFlashCountdown > 0)
    {
        mBeghouledFlashCountdown--;
    }

    if (mSquished)
    {
        mFrame = 0;
        return;
    }

    if (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_TALLNUT || 
        mSeedType == SeedType::SEED_EXPLODE_O_NUT || mSeedType == SeedType::SEED_DOOM_NUT)
    {
        AnimateNuts();
    }
    else if (mSeedType == SeedType::SEED_GARLIC)
    {
        AnimateGarlic();
    }
    else if (mSeedType == SeedType::SEED_CORROSION_GARLIC)
    {
        AnimateCorrosionGarlic();
    }
    else if (mSeedType == SeedType::SEED_PUMPKINSHELL)
    {
        AnimatePumpkin();
    }

    UpdateBlink();

    if (mAnimPing)
    {
        if (mAnimCounter < mFrameLength * mNumFrames - 1)
        {
            mAnimCounter++;
        }
        else
        {
            mAnimPing = false;
            mAnimCounter -= mFrameLength;
        }
    }
    else if (mAnimCounter > 0)
    {
        mAnimCounter--;
    }
    else
    {
        mAnimPing = true;
        mAnimCounter += mFrameLength;
    }
    mFrame = mAnimCounter / mFrameLength;
}

float PlantFlowerPotHeightOffset(SeedType theSeedType, float theFlowerPotScale)
{
    float aHeightOffset = -13.0f * theFlowerPotScale;
    float aScaleOffsetFix = 0.0f;

    switch (theSeedType)
    {
    case SeedType::SEED_CHOMPER:
    case SeedType::SEED_PLANTERN:
    case SeedType::SEED_SUPER_CHOMPER:
    case SeedType::SEED_ICE_PLANTERN:
        aHeightOffset -= 5.0f;
        break;
    case SeedType::SEED_SCAREDYSHROOM:
        aHeightOffset += 5.0f;
        aScaleOffsetFix -= 8.0f;
        break;
    case SeedType::SEED_SUNSHROOM:
    case SeedType::SEED_PUFFSHROOM:
        aScaleOffsetFix -= 4.0f;
        break;
    case SeedType::SEED_HYPNOSHROOM:
    case SeedType::SEED_MAGNETSHROOM:
    case SeedType::SEED_PEASHOOTER:
    case SeedType::SEED_REPEATER:
    case SeedType::SEED_LEFTPEATER:
    case SeedType::SEED_SNOWPEA:
    case SeedType::SEED_THREEPEATER:
    case SeedType::SEED_SUNFLOWER:
    case SeedType::SEED_MARIGOLD:
    case SeedType::SEED_CABBAGEPULT:
    case SeedType::SEED_MELONPULT:
    case SeedType::SEED_TANGLEKELP:
    case SeedType::SEED_BLOVER:
    case SeedType::SEED_SPIKEWEED:
    case SeedType::SEED_FIRESHOOTER:
    case SeedType::SEED_ICEPEA:
    case SeedType::SEED_HYPNOGIGA:
    case SeedType::SEED_BLACK_THREEPEATER:
    case SeedType::SEED_POISONPULT:
    case SeedType::SEED_COMMANDOPEA:
    case SeedType::SEED_SHRINKING_VIOLET:
    case SeedType::SEED_HATTREMWITCH:
    case SeedType::SEED_NIGHTCAP:
    case SeedType::SEED_SWEETPOTATO:
    case SeedType::SEED_GENERALPEA:
    case SeedType::SEED_HATTREMSAGE:
    case SeedType::SEED_DARKCAP:
        aScaleOffsetFix -= 8.0f;
        break;
    case SeedType::SEED_SEASHROOM:
    case SeedType::SEED_POTATOMINE:
    case SeedType::SEED_RED_POTATO_MINE:
        aScaleOffsetFix -= 4.0f;
        break;
    case SeedType::SEED_LILYPAD:
        aScaleOffsetFix -= 16.0f;
        break;
    case SeedType::SEED_INSTANT_COFFEE:
        aScaleOffsetFix -= 20.0f;
        break;
    }

    return aHeightOffset + (theFlowerPotScale * aScaleOffsetFix - aScaleOffsetFix);
}

float PlantDrawHeightOffset(Board* theBoard, Plant* thePlant, SeedType theSeedType, int theCol, int theRow)
{
    float aHeightOffset = 0.0f;

    bool doFloating = false;
    if (Plant::IsFlying(theSeedType))
    {
        doFloating = false;
    }
    else if (theBoard == nullptr)
    {
        if (Plant::IsAquatic(theSeedType))
        {
            doFloating = true;
        }
    }
    else if (theBoard->IsPoolSquare(theCol, theRow))
    {
        doFloating = true;
    }

    if (doFloating)
    {
        int aCounter;
        if (theBoard)
        {
            aCounter = theBoard->mMainCounter;
        }
        else
        {
            aCounter = gLawnApp->mAppCounter;
        }

        float aPos = theRow * PI + theCol * 0.25f * PI;
        float aTime = aCounter * 2.0f * PI / 200.0f;
        float aFloatingHeight = sin(aPos + aTime) * 2.0f;
        aHeightOffset += aFloatingHeight;
    }

    if (theBoard && (thePlant == nullptr || !thePlant->mSquished))
    {
        Plant* aPot = theBoard->GetFlowerPotAt(theCol, theRow);
        if (aPot && !aPot->mSquished && theSeedType != SeedType::SEED_FLOWERPOT)
        {
            aHeightOffset += PlantFlowerPotHeightOffset(theSeedType, 1.0f);
        }
    }

    if (theSeedType == SeedType::SEED_FLOWERPOT)
    {
        aHeightOffset += 26.0f;
    }
    else if (theSeedType == SeedType::SEED_LILYPAD)
    {
        aHeightOffset += 25.0f;
    }
    else if (theSeedType == SeedType::SEED_STARFRUIT || theSeedType == SeedType::SEED_SOUR_STARFRUIT)
    {
        aHeightOffset += 10.0f;
    }
    else if (theSeedType == SeedType::SEED_TANGLEKELP)
    {
        aHeightOffset += 24.0f;
    }
    else if (theSeedType == SeedType::SEED_SEASHROOM)
    {
        aHeightOffset += 28.0f;
    }
    else if (theSeedType == SeedType::SEED_INSTANT_COFFEE)
    {
        aHeightOffset -= 20.0f;
    }
    //else if (Plant::IsFlying(theSeedType))
    //{
    //    aHeightOffset -= 30.0f;
    //}
    else if (theSeedType == SeedType::SEED_CACTUS || theSeedType == SeedType::SEED_MAD_CACTUS)
    {
        return aHeightOffset;
    }
    else if (theSeedType == SeedType::SEED_PUMPKINSHELL)
    {
        aHeightOffset += 15.0f;
    }
    else if (theSeedType == SeedType::SEED_PUFFSHROOM)
    {
        aHeightOffset += 5.0f;
    }
    else if (theSeedType == SeedType::SEED_SCAREDYSHROOM)
    {
        aHeightOffset -= 14.0f;
    }
    else if (theSeedType == SeedType::SEED_GRAVEBUSTER)
    {
        aHeightOffset -= 40.0f;
    }
    else if (theSeedType == SeedType::SEED_SPIKEWEED || theSeedType == SeedType::SEED_SPIKEROCK)
    {
        int aBottomRow = 4;
        if (theBoard && theBoard->StageHas6Rows())
        {
            aBottomRow = 5;
        }

        if (theSeedType == SeedType::SEED_SPIKEROCK)
        {
            aHeightOffset += 6.0f;
        }

        if (theBoard && theBoard->GetFlowerPotAt(theCol, theRow) && gLawnApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
        {
            aHeightOffset += 5.0f;
        }
        else if (theBoard && theBoard->StageHasRoof())
        {
            aHeightOffset += 15.0f;
        }
        else if (theBoard && theBoard->IsPoolSquare(theCol, theRow))
        {
            aHeightOffset += 0.0f;
        }
        else if (theRow == aBottomRow && theCol >= 7 && theBoard->StageHas6Rows())
        {
            aHeightOffset += 1.0f;
        }
        else if (theRow == aBottomRow && theCol < 7)
        {
            aHeightOffset += 12.0f;
        }
        else
        {
            aHeightOffset += 15.0f;
        }
    }

    return aHeightOffset;
}

void Plant::GetPeaHeadOffset(int& theOffsetX, int& theOffsetY)
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);

    int aTrackIndex = 0;
    if (aBodyReanim->TrackExists("anim_stem"))
    {
        aTrackIndex = aBodyReanim->FindTrackIndex("anim_stem");
    }
    else if(aBodyReanim->TrackExists("anim_idle"))
    {
        aTrackIndex = aBodyReanim->FindTrackIndex("anim_idle");
    }

    ReanimatorTransform aTransform;
    aBodyReanim->GetCurrentTransform(aTrackIndex, &aTransform);
    theOffsetX = aTransform.mTransX;
    theOffsetY = aTransform.mTransY;
}

void Plant::DrawMagnetItems(Graphics* g)
{
    float aOffsetX = 0.0f;
    float aOffsetY = PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);
    static Reanimation* sSunReanim = nullptr;

    for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
    {
        MagnetItem* aMagnetItem = &mMagnetItems[i];
        if (aMagnetItem->mItemType != MagnetItemType::MAGNET_ITEM_NONE)
        {
            int aCelRow = 0, aCelCol = 0;
            Image* aImage = nullptr;
            float aScale = 0.8f;

            if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_PAIL_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BUCKET1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_PAIL_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BUCKET2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_PAIL_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BUCKET3;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_FOOTBALL_HELMET_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_FOOTBALL_HELMET_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_FOOTBALL_HELMET_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET3;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_DOOR_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_SCREENDOOR1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_DOOR_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_SCREENDOOR2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_DOOR_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_SCREENDOOR3;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_BLACK_CONE_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BLACK_CONE1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_BLACK_CONE_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BLACK_CONE2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_BLACK_CONE_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BLACK_CONE3;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_BLACK_PAIL_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BLACK_BUCKET1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_BLACK_PAIL_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BLACK_BUCKET2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_BLACK_PAIL_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BLACK_BUCKET3;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_BLACK_DOOR_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BLACK_SCREENDOOR1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_BLACK_DOOR_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BLACK_SCREENDOOR2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_BLACK_DOOR_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BLACK_SCREENDOOR3;
            }
            else if (aMagnetItem->mItemType >= MagnetItemType::MAGNET_ITEM_POGO_1 && aMagnetItem->mItemType <= MagnetItemType::MAGNET_ITEM_POGO_3)
            {
                aCelCol = (int)aMagnetItem->mItemType - (int)MagnetItemType::MAGNET_ITEM_POGO_1;
                aImage = IMAGE_ZOMBIEPOGO;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_LADDER_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_LADDER_1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_LADDER_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_LADDER_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_LADDER_PLACED)
            {
                aImage = IMAGE_REANIM_ZOMBIE_LADDER_5;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_JACK_IN_THE_BOX)
            {
                aImage = IMAGE_REANIM_ZOMBIE_JACKBOX_BOX;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_PICK_AXE)
            {
                aImage = IMAGE_REANIM_ZOMBIE_DIGGER_PICKAXE;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_SILVER_COIN)
            {
                aScale = 1.0f;
                aImage = IMAGE_REANIM_COIN_SILVER_DOLLAR;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_GOLD_COIN)
            {
                aScale = 1.0f;
                aImage = IMAGE_REANIM_COIN_GOLD_DOLLAR;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_DIAMOND)
            {
                aScale = 1.0f;
                aImage = IMAGE_REANIM_DIAMOND;
            }
            else if (aMagnetItem->mItemType == MAGNET_ITEM_NORMAL_SUN)
            {
                aScale = 1.0f;
                if (sSunReanim == nullptr)
                {
                    sSunReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_SUN);
                    sSunReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
                    sSunReanim->mAnimRate = 6.0f;
                }

                float magnetTargetX = 200.0f;
                float magnetTargetY = 200.0f;

                sSunReanim->SetPosition(magnetTargetX, magnetTargetY);
                aImage = sSunReanim->GetCurrentTrackImage("TrackName");
            }
            else if (aMagnetItem->mItemType == MAGNET_ITEM_SMALL_SUN)
            {
                aScale = 0.5f;
                if (sSunReanim == nullptr)
                {
                    sSunReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_SUN);
                    sSunReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
                    sSunReanim->mAnimRate = 6.0f;
                }

                float magnetTargetX = 20.0f;
                float magnetTargetY = 20.0f;

                sSunReanim->SetPosition(magnetTargetX, magnetTargetY);
                aImage = sSunReanim->GetCurrentTrackImage("TrackName");
            }
            else if (aMagnetItem->mItemType == MAGNET_ITEM_LARGE_SUN)
            {
                aScale = 2.0f;
                if (sSunReanim == nullptr)
                {
                    sSunReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_SUN);
                    sSunReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
                    sSunReanim->mAnimRate = 6.0f;
                }

                float magnetTargetX = 20.0f;
                float magnetTargetY = 20.0f;

                sSunReanim->SetPosition(magnetTargetX, magnetTargetY);
                aImage = sSunReanim->GetCurrentTrackImage("TrackName");
            }
            
            else if (aMagnetItem->mItemType == MAGNET_ITEM_WHITE_SUN)
            {
                aScale = 1.0f;
                if (sSunReanim == nullptr)
                {
                    sSunReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_SUN);
                    sSunReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
                    sSunReanim->mAnimRate = 6.0f;
                }

                float magnetTargetX = 20.0f;
                float magnetTargetY = 20.0f;

                sSunReanim->mColorOverride = Color(255, 255, 255, 255);
                sSunReanim->mFilterEffect = FilterEffect::FILTER_EFFECT_WHITE;
                sSunReanim->mExtraOverlayColor = Color(200, 200, 200, 150);

                sSunReanim->SetPosition(magnetTargetX, magnetTargetY);
                aImage = sSunReanim->GetCurrentTrackImage("TrackName");
            }
            else if (aMagnetItem->mItemType == MAGNET_ITEM_BIG_WHITE_SUN)
            {
                aScale = 2.0f;
                if (sSunReanim == nullptr)
                {
                    sSunReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_SUN);
                    sSunReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
                    sSunReanim->mAnimRate = 6.0f;
                }

                float magnetTargetX = 20.0f;
                float magnetTargetY = 20.0f;

                sSunReanim->mColorOverride = Color(255, 255, 255, 255);
                sSunReanim->mFilterEffect = FilterEffect::FILTER_EFFECT_WHITE;
                sSunReanim->mExtraOverlayColor = Color(200, 200, 200, 150);

                aImage = sSunReanim->GetCurrentTrackImage("TrackName");
            }
            else
            {
                TOD_ASSERT();
            }

            if (aScale == 1.0f || aScale == 0.5f)
            {
                if (aMagnetItem->mItemType == MAGNET_ITEM_NORMAL_SUN || aMagnetItem->mItemType == MAGNET_ITEM_SMALL_SUN || aMagnetItem->mItemType == MAGNET_ITEM_WHITE_SUN)
                {
                    g->DrawImageCel(aImage, aMagnetItem->mPosX - mX - 10, aMagnetItem->mPosY - mY - 70, aCelCol, aCelRow);
                }
                else
                {
                    g->DrawImageCel(aImage, aMagnetItem->mPosX - mX + aOffsetX, aMagnetItem->mPosY - mY + aOffsetY, aCelCol, aCelRow);
                }
            }
            else
            {
                if (aMagnetItem->mItemType == MAGNET_ITEM_LARGE_SUN || aMagnetItem->mItemType == MAGNET_ITEM_BIG_WHITE_SUN)
                {
                    TodDrawImageCelScaledF(g, aImage, aMagnetItem->mPosX - mX - 50, aMagnetItem->mPosY - mY - 160, aCelCol, aCelRow, aScale, aScale);
                }
                else
                {
                    TodDrawImageCelScaledF(g, aImage, aMagnetItem->mPosX - mX + aOffsetX, aMagnetItem->mPosY - mY + aOffsetY, aCelCol, aCelRow, aScale, aScale);
                }
            }
        }
    }
}

Image* Plant::GetImage(SeedType theSeedType)
{
    Image** aImages = GetPlantDefinition(theSeedType).mPlantImage;
    return aImages ? aImages[0] : nullptr;
}

void Plant::DrawShadow(Sexy::Graphics* g, float theOffsetX, float theOffsetY)
{
    if (mSeedType == SeedType::SEED_LILYPAD || mSeedType == SeedType::SEED_STARFRUIT || mSeedType == SeedType::SEED_TANGLEKELP || 
        mSeedType == SeedType::SEED_SEASHROOM || mSeedType == SeedType::SEED_COBCANNON || mSeedType == SeedType::SEED_SPIKEWEED || 
        mSeedType == SeedType::SEED_SPIKEROCK || mSeedType == SeedType::SEED_GRAVEBUSTER || mSeedType == SeedType::SEED_CATTAIL || 
        mSeedType == SeedType::SEED_SOUR_STARFRUIT ||
        mOnBungeeState == PlantOnBungeeState::RISING_WITH_BUNGEE)
        return;

    if (IsOnBoard() && mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN && mApp->mZenGarden->mGardenType == GardenType::GARDEN_MAIN)
        return;

    int aShadowType = 0;
    float aShadowOffsetX = -3.0f;
    float aShadowOffsetY = 51.0f;
    float aScale = 1.0f;
    if (mBoard && mBoard->StageIsNight())
    {
        aShadowType = 1;
    }

    if (mSeedType == SeedType::SEED_SQUASH || mSeedType == SeedType::SEED_BOMBSQUASH)
    {
        if (mBoard)
        {
            aShadowOffsetY += mBoard->GridToPixelY(mPlantCol, mRow) - mY;
        }
        aShadowOffsetY += 5.0f;
    }
    else if (mSeedType == SeedType::SEED_PUFFSHROOM)
    {
        aScale = 0.5f;
        aShadowOffsetY = 42.0f;
    }
    else if (mSeedType == SeedType::SEED_SUNSHROOM)
    {
        aShadowOffsetY = 42.0f;
        if (mState == PlantState::STATE_SUNSHROOM_SMALL)
        {
            aScale = 0.5f;
        }
        else if (mState == PlantState::STATE_SUNSHROOM_GROWING)
        {
            Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
            aScale = 0.5f + 0.5f * aBodyReanim->mAnimTime;
        }
    }
    else if (mSeedType == SeedType::SEED_UMBRELLA || mSeedType == SeedType::SEED_ABSOLUTELEAF)
    {
        aScale = 0.5f;
        aShadowOffsetX = -7.0f;
        aShadowOffsetY = 52.0f;
    }
    else if (mSeedType == SeedType::SEED_FUMESHROOM || mSeedType == SeedType::SEED_GLOOMSHROOM)
    {
        aScale = 1.3f;
        aShadowOffsetY = 47.0f;
    }
    else if (mSeedType == SeedType::SEED_CABBAGEPULT || mSeedType == SeedType::SEED_MELONPULT || mSeedType == SeedType::SEED_WINTERMELON || mSeedType == SeedType::SEED_POISONPULT)
    {
        aShadowOffsetY = 47.0f;
    }
    else if (mSeedType == SeedType::SEED_KERNELPULT)
    {
        aShadowOffsetX = 0.0f;
        aShadowOffsetY = 47.0f;
    }
    else if (mSeedType == SeedType::SEED_SCAREDYSHROOM)
    {
        aShadowOffsetX = -9.0f;
        aShadowOffsetY = 55.0f;
    }
    else if (mSeedType == SeedType::SEED_CHOMPER || mSeedType == SeedType::SEED_SUPER_CHOMPER)
    {
        aShadowOffsetX = -21.0f;
        aShadowOffsetY = 57.0f;
    }
    else if (mSeedType == SeedType::SEED_FLOWERPOT)
    {
        aShadowOffsetX = -4.0f;
        aShadowOffsetY = 46.0f;
    }
    else if (mSeedType == SeedType::SEED_TALLNUT || mSeedType == SeedType::SEED_DOOM_NUT)
    {
        aShadowOffsetY = 54.0f;
        aScale = 1.3f;
    }
    else if (mSeedType == SeedType::SEED_PUMPKINSHELL)
    {
        aShadowOffsetY = 46.0f;
        aScale = 1.4f;
    }
    else if (mSeedType == SeedType::SEED_CACTUS || mSeedType == SeedType::SEED_MAD_CACTUS)
    {
        aShadowOffsetX = -8.0f;
        aShadowOffsetY = 50.0f;
    }
    else if (mSeedType == SeedType::SEED_PLANTERN || mSeedType == SeedType::SEED_ICE_PLANTERN)
    {
        aShadowOffsetY = 57.0f;
    }
    else if (mSeedType == SeedType::SEED_INSTANT_COFFEE)
    {
        aShadowOffsetY = 71.0f;
    }
    else if (mSeedType == SeedType::SEED_GIANT_WALLNUT)
    {
        aShadowOffsetX = -33.0f;
        aShadowOffsetY = 56.0f;
        aScale = 1.7f;
    }

    if (Plant::IsFlying(mSeedType))
    {
        aShadowOffsetY += 10.0f;
        if (mBoard && (mBoard->GetTopPlantAt(mPlantCol, mRow, TOPPLANT_ONLY_NORMAL_POSITION) || mBoard->GetTopPlantAt(mPlantCol, mRow, TOPPLANT_ONLY_PUMPKIN)))
            return;
    }

    if (aShadowType == 0)
    {
        TodDrawImageCelCenterScaledF(g, IMAGE_PLANTSHADOW, theOffsetX + aShadowOffsetX, theOffsetY + aShadowOffsetY, 0, aScale, aScale);
    }
    else
    {
        TodDrawImageCelCenterScaledF(g, IMAGE_PLANTSHADOW2, theOffsetX + aShadowOffsetX, theOffsetY + aShadowOffsetY, 0, aScale, aScale);
    }
}

void Plant::Draw(Graphics* g)
{
    float aOffsetX = 0.0f;
    float aOffsetY = PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);
    if (Plant::IsFlying(mSeedType) && mSquished)
    {
        aOffsetY += 30.0f;
    }
    if (mFrozenCounter > 0)
    {
        g->SetColorizeImages(true);
        g->SetColor(Color(75, 75, 255, 255)); // Blue tint
    }

    int aImageIndex = mFrame;
    Image* aPlantImage = Plant::GetImage(mSeedType);

    if (mSquished)
    {
        if (mSeedType == SeedType::SEED_FLOWERPOT)
        {
            aOffsetY -= 15.0f;
        }
        if (mSeedType == SeedType::SEED_INSTANT_COFFEE)
        {
            aOffsetY -= 20.0f;
        }

        g->SetScale(1.0f, 0.25f, 0.0f, 0.0f);
        DrawSeedType(g, mSeedType, mImitaterType, DrawVariation::VARIATION_NORMAL, aOffsetX, 60.0f + aOffsetY);
        g->SetScale(1.0f, 1.0f, 0.0f, 0.0f);
    }
    else
    {
        bool aDrawPumpkinBack = false;
        Plant* aPumpkin = nullptr;

        if (IsOnBoard())
        {
            aPumpkin = mBoard->GetPumpkinAt(mPlantCol, mRow);
            if (aPumpkin)
            {
                Plant* aPlantInPumpkin = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
                if (aPlantInPumpkin)
                {
                    if (aPlantInPumpkin->mRenderOrder > aPumpkin->mRenderOrder || aPlantInPumpkin->mOnBungeeState == GETTING_GRABBED_BY_BUNGEE)
                    {
                        aPlantInPumpkin = nullptr;
                    }
                }

                if (aPlantInPumpkin == this)
                {
                    aDrawPumpkinBack = true;
                }
                if (aPlantInPumpkin == nullptr && mSeedType == SeedType::SEED_PUMPKINSHELL)
                {
                    aDrawPumpkinBack = true;
                }
            }
            else if (mSeedType == SeedType::SEED_PUMPKINSHELL)
            {
                aDrawPumpkinBack = true;
                aPumpkin = this;
            }
        }
        else if (mSeedType == SeedType::SEED_PUMPKINSHELL)
        {
            aDrawPumpkinBack = true;
            aPumpkin = this;
        }

        DrawShadow(g, aOffsetX, aOffsetY);

        if (Plant::IsFlying(mSeedType))
        {
            int aCounter;
            if (IsOnBoard())
            {
                aCounter = mBoard->mMainCounter;
            }
            else
            {
                aCounter = mApp->mAppCounter;
            }

            float aTime = (mRow * 97 + mPlantCol * 61 + aCounter) * 0.03f;
            float aWave = sin(aTime) * 2.0f;
            aOffsetY += aWave;
        }

        if (aDrawPumpkinBack)
        {
            Reanimation* aPumpkinReanim = mApp->ReanimationGet(aPumpkin->mBodyReanimID);
            Graphics aPumpkinGraphics(*g);
            aPumpkinGraphics.mTransX += aPumpkin->mX - mX;
            aPumpkinGraphics.mTransY += aPumpkin->mY - mY;
            aPumpkinReanim->DrawRenderGroup(&aPumpkinGraphics, 1);
        }

        aOffsetX += mShakeOffsetX;
        aOffsetY += mShakeOffsetY;
        if (IsInPlay() && mApp->IsIZombieLevel())
        {
            mBoard->mChallenge->IZombieDrawPlant(g, this);
        }
        else if (mBodyReanimID != ReanimationID::REANIMATIONID_NULL)
        {
            Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
            if (aBodyReanim)
            {
                if (!mApp->Is3dAccel() && mSeedType == SeedType::SEED_FLOWERPOT && IsOnBoard() &&
                    aBodyReanim->mAnimRate == 0.0f && aBodyReanim->IsAnimPlaying("anim_idle"))
                {
                    mApp->mReanimatorCache->DrawCachedPlant(g, aOffsetX, aOffsetY, mSeedType, DrawVariation::VARIATION_NORMAL);
                }
                else
                {
                    aBodyReanim->Draw(g);
                }
            }
        }
        else
        {
            SeedType aSeedType = SeedType::SEED_NONE;
            if (mBoard)
            {
                aSeedType = mBoard->GetSeedTypeInCursor();
            }

            if (IsPartOfUpgradableTo(aSeedType) && mBoard->CanPlantAt(mPlantCol, mRow, aSeedType) == PlantingReason::PLANTING_OK)
            {
                g->SetColorizeImages(true);
                g->SetColor(GetFlashingColor(mBoard->mMainCounter, 90));
            }
            else if (mSeedType == SeedType::SEED_SUNSHROOM && aSeedType == SeedType::SEED_SUNSHROOM && mBoard->CanPlantAt(mPlantCol, mRow, SeedType::SEED_SUNSHROOM) == PlantingReason::PLANTING_OK)
            {
                g->SetColorizeImages(true);
                g->SetColor(GetFlashingColor(mBoard->mMainCounter, 90));
            }
            else if (mSeedType == SeedType::SEED_SCAREDYSHROOM && mBoard && mBoard->mApp->IsAdventureMode() && mBoard->mLevel == 17 && !mBoard->mScaredyShroomTutorialCompleted && this == mBoard->GetFirstPlantByType(SeedType::SEED_SCAREDYSHROOM))
            {
                g->SetColorizeImages(true);
                g->SetColor(GetFlashingColor(mBoard->mMainCounter, 90));
            }
            else if (aSeedType == SeedType::SEED_COBCANNON && mBoard->CanPlantAt(mPlantCol - 1, mRow, aSeedType) == PlantingReason::PLANTING_OK)
            {
                g->SetColorizeImages(true);
                g->SetColor(GetFlashingColor(mBoard->mMainCounter, 90));
            }
            else if (mBoard && mBoard->mTutorialState == TutorialState::TUTORIAL_SHOVEL_DIG)
            {
                g->SetColorizeImages(true);
                g->SetColor(GetFlashingColor(mBoard->mMainCounter, 90));
            }

            TodDrawImageCelF(g, aPlantImage, aOffsetX, aOffsetY, aImageIndex, 0);
            g->SetColorizeImages(false);
            if (mHighlighted)
            {
                g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
                g->SetColorizeImages(true);
                g->SetColor(Color(255, 255, 255, 196));
                TodDrawImageCelF(g, aPlantImage, aOffsetX, aOffsetY, aImageIndex, 0);
                g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
                g->SetColorizeImages(false);
            }
            else if (mEatenFlashCountdown > 0)
            {
                g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
                g->SetColorizeImages(true);
                g->SetColor(Color(255, 255, 255, ClampInt(mEatenFlashCountdown * 3, 0, 255)));
                TodDrawImageCelF(g, aPlantImage, aOffsetX, aOffsetY, aImageIndex, 0);
                g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
                g->SetColorizeImages(false);
            }
        }

        if (mSeedType == SeedType::SEED_MAGNETSHROOM && !DrawMagnetItemsOnTop())
        {
            DrawMagnetItems(g);
        }
    }

    if ((mSeedType == SEED_PLANTERN || mSeedType == SEED_ICE_PLANTERN) && !mIsAsleep && !mSquished && IsInPlay())
    {
        g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
        g->SetColorizeImages(true);

        // Define the aura color and transparency
        Color auraColor;
        if (mSeedType == SEED_ICE_PLANTERN)
        {
            auraColor = Color(120, 120, 255, 100); // A soft blue for Ice Plantern
        }
        else
        {
            auraColor = Color(255, 255, 120, 100); // A soft yellow for Plantern
        }
        g->SetColor(auraColor);

        // Calculate position and scale for the new circular image
        float centerX = aOffsetX + 40.0f; // Center of the plant's tile
        float centerY = aOffsetY + 40.0f;
        float scale = (mSeedType == SEED_ICE_PLANTERN) ? 2.5f : 1.2f;

        // Use the Doom-shroom's explosion base for a perfect circular aura
        TodDrawImageCelScaledF(g, IMAGE_DOOMSHROOM_EXPLOSION_BASE,
            centerX - (IMAGE_DOOMSHROOM_EXPLOSION_BASE->GetCelWidth() * scale / 2),
            centerY - (IMAGE_DOOMSHROOM_EXPLOSION_BASE->GetCelHeight() * scale / 2),
            0, 0, scale, scale);

        // Reset graphics state
        g->SetColorizeImages(false);
        g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
    }

    if (mSeedType == SeedType::SEED_SUPER_SPLITPEA && !mIsAsleep && !mSquished && mBoard != nullptr && IsInPlay())
    {
        g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
        g->SetColorizeImages(true);

        int pulseAlpha = 80 + (int)(sinf((float)mBoard->mMainCounter * 0.1f) * 40.0f);
        g->SetColor(Color(255, 60, 255, pulseAlpha));

        float centerX = aOffsetX + 40.0f;
        float centerY = aOffsetY + 40.0f;
        float scale = 1.4f + sinf((float)mBoard->mMainCounter * 0.05f) * 0.1f;

        TodDrawImageCelScaledF(g, IMAGE_DOOMSHROOM_EXPLOSION_BASE,
            centerX - (IMAGE_DOOMSHROOM_EXPLOSION_BASE->GetCelWidth() * scale / 2),
            centerY - (IMAGE_DOOMSHROOM_EXPLOSION_BASE->GetCelHeight() * scale / 2),
            0, 0, scale, scale);

        g->SetColorizeImages(false);
        g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
    }



    if (mSeedType == SeedType::SEED_BLACK_THREEPEATER)
    {
        if (mBlackThreepeaterSpecialBurstCount > 0)
        {
            mBlackThreepeaterSpecialBurstTimer--;
            if (mBlackThreepeaterSpecialBurstTimer <= 0)
            {
                FireBlackThreepeater();
                mBlackThreepeaterSpecialBurstCount--;
                mBlackThreepeaterSpecialBurstTimer = 12; // 12-tick delay between bursts
            }
        }
    }

    if (mShieldHealth > 0 && !mSquished && IsInPlay())
    {
        float shieldIconX = aOffsetX + 32.0f;
        float shieldIconY = aOffsetY - 15.0f;
        g->SetColor(Color(0, 40, 100, 220));
        g->FillRect(Rect((int)shieldIconX - 1, (int)shieldIconY - 1, 16, 16));
        g->SetColor(Color(0, 200, 255, 255));
        g->DrawRect(Rect((int)shieldIconX - 1, (int)shieldIconY - 1, 16, 16));
        g->SetColor(Color::White);
        g->FillRect(Rect((int)shieldIconX + 5, (int)shieldIconY + 3, 4, 8));
        g->FillRect(Rect((int)shieldIconX + 3, (int)shieldIconY + 5, 8, 4));
    }

    if (mGreenFilterEffect > 0)
    {
        Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
        if (aBodyReanim)
        {
            g->SetColorizeImages(true);
            g->SetColor(Color(100, 255, 100, mGreenFilterEffect)); // Green tint with alpha
            aBodyReanim->Draw(g); // Redraw the plant with the color filter
            g->SetColorizeImages(false);
        }
    }

    if (mButteredCounter > 0)
    {
        Image* butterImage = IMAGE_REANIM_CORNPULT_BUTTER_SPLAT;
        float butterWidth = butterImage->GetCelWidth() * 0.8f;
        float butterHeight = butterImage->GetCelHeight() * 0.8f;

        // Draw the butter centered on the stored impact coordinates
        TodDrawImageCelScaledF(g, butterImage, mButterX - (butterWidth / 2.0f), mButterY - (butterHeight / 2.0f), 0, 0, 0.8f, 0.8f);
    }
}

void Plant::DrawSeedType(Graphics* g, SeedType theSeedType, SeedType theImitaterType, DrawVariation theDrawVariation, float thePosX, float thePosY)
{
    if (theSeedType == SEED_NONE)
    {
        return;
    }

    Graphics aSeedG(*g);
    int aCelRow = 0;
    int aCelCol = 2;
    float aOffsetX = 0.0f;
    float aOffsetY = 0.0f;
    SeedType aSeedType = theSeedType;
    DrawVariation aDrawVariation = theDrawVariation;

    if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE)
    {
        aSeedType = theImitaterType;
        aDrawVariation = DrawVariation::VARIATION_IMITATER;
        if (theImitaterType == SeedType::SEED_HYPNOSHROOM || theImitaterType == SeedType::SEED_SQUASH || theImitaterType == SeedType::SEED_POTATOMINE ||
            theImitaterType == SeedType::SEED_GARLIC || theImitaterType == SeedType::SEED_LILYPAD)
            aDrawVariation = DrawVariation::VARIATION_IMITATER_LESS;
    }
    else if (theDrawVariation == DrawVariation::VARIATION_NORMAL && theSeedType == SeedType::SEED_TANGLEKELP)
    {
        aDrawVariation = DrawVariation::VARIATION_AQUARIUM;
    }

    if (((LawnApp*)gSexyAppBase)->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME &&
        (aSeedType == SeedType::SEED_WALLNUT || aSeedType == SeedType::SEED_SUNFLOWER || aSeedType == SeedType::SEED_MARIGOLD))
    {
        aSeedG.mScaleX *= 1.5f;
        aSeedG.mScaleY *= 1.5f;
        aOffsetX = -20.0f;
        aOffsetY = -40.0f;
    }
    if (aSeedType == SeedType::SEED_LEFTPEATER)
    {
        aOffsetX += aSeedG.mScaleX * 80.0f;
        aSeedG.mScaleX *= -1.0f;
    }

    if (Challenge::IsZombieSeedType(aSeedType))
    {
        ZombieType aZombieType = Challenge::IZombieSeedTypeToZombieType(aSeedType);
        gLawnApp->mReanimatorCache->DrawCachedZombie(&aSeedG, thePosX + aOffsetX, thePosY + aOffsetY, aZombieType);
    }
    else
    {
        const PlantDefinition& aPlantDef = GetPlantDefinition(aSeedType);

        if (aSeedType == SeedType::SEED_GIANT_WALLNUT)
        {
            aSeedG.mScaleX *= 1.4f;
            aSeedG.mScaleY *= 1.4f;
            TodDrawImageScaledF(&aSeedG, IMAGE_REANIM_WALLNUT_BODY, thePosX - 53.0f, thePosY - 56.0f, aSeedG.mScaleX, aSeedG.mScaleY);
        }
        else if (aPlantDef.mReanimationType != ReanimationType::REANIM_NONE)
        {
            gLawnApp->mReanimatorCache->DrawCachedPlant(&aSeedG, thePosX + aOffsetX, thePosY + aOffsetY, aSeedType, aDrawVariation);
        }
        else
        {
            if (aSeedType == SeedType::SEED_KERNELPULT)
            {
                aCelRow = 2;
            }
            else if (aSeedType == SeedType::SEED_TWINSUNFLOWER)
            {
                aCelRow = 1;
            }

            Image* aPlantImage = Plant::GetImage(aSeedType);
            if (aPlantImage->mNumCols <= 2)
            {
                aCelCol = aPlantImage->mNumCols - 1;
            }

            TodDrawImageCelScaledF(&aSeedG, aPlantImage, thePosX + aOffsetX, thePosY + aOffsetY, aCelCol, aCelRow, aSeedG.mScaleX, aSeedG.mScaleY);
        }
    }
}

void Plant::MouseDown(int x, int y, int theClickCount)
{
    if (theClickCount == -1 && mSeedType == SEED_SCAREDYSHROOM)
    {
        mIsRandom = !mIsRandom; // Toggle the random mode on/off
        mApp->PlayFoley(FOLEY_BLEEP);
        if (mBoard && mBoard->mScaredyShroomTutorialShowing)
        {
            mBoard->mScaredyShroomTutorialShowing = false;
            mBoard->mScaredyShroomTutorialCompleted = true;
            mBoard->TutorialArrowRemove();
            mBoard->ClearAdviceImmediately();
        }
        return;
    }

    if (theClickCount < 0)
        return;

    if (mState == PlantState::STATE_COBCANNON_READY)
    {
        mBoard->ClearCursor();
        mBoard->mCursorObject->mType = SeedType::SEED_NONE;
        mBoard->mCursorObject->mCursorType = CursorType::CURSOR_TYPE_COBCANNON_TARGET;
        mBoard->mCursorObject->mSeedBankIndex = -1;
        mBoard->mCursorObject->mCoinID = CoinID::COINID_NULL;
        mBoard->mCursorObject->mCobCannonPlantID = (PlantID)mBoard->mPlants.DataArrayGetID(this);
        mBoard->mCobCannonCursorDelayCounter = 30;
        mBoard->mCobCannonMouseX = x;
        mBoard->mCobCannonMouseY = y;
    }
}

void Plant::IceZombies()
{
    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        aZombie->HitIceTrap();
    }

    mBoard->mIceTrapCounter = 300;
    TodParticleSystem* aPoolSparklyParticle = mApp->ParticleTryToGet(mBoard->mPoolSparklyParticleID);
    if (aPoolSparklyParticle)
    {
        aPoolSparklyParticle->mDontUpdate = false;
    }

    Zombie* aBossZombie = mBoard->GetBossZombie();
    if (aBossZombie)
    {
        aBossZombie->BossDestroyFireball();
    }
}

void Plant::BurnRow(int theRow)
{
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if ((aZombie->mZombieType == ZombieType::ZOMBIE_BOSS || aZombie->mRow == theRow) && aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            aZombie->RemoveColdEffects();
            aZombie->ApplyBurn();
        }
    }

    GridItem* aGridItem = nullptr;
    while (mBoard->IterateGridItems(aGridItem))
    {
        if (aGridItem->mGridY == theRow && aGridItem->mGridItemType == GridItemType::GRIDITEM_LADDER)
        {
            aGridItem->GridItemDie();
        }
    }

    Zombie* aBossZombie = mBoard->GetBossZombie();
    if (aBossZombie && aBossZombie->mFireballRow == theRow)
    {
        aBossZombie->BossDestroyIceballInRow(theRow);
    }
}

void Plant::BurnColumn(int theCol)
{
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
    Rect aColumnRect(mBoard->GridToPixelX(theCol, 0) - 40, 0, 80, BOARD_HEIGHT);

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) 
        {
            aZombie->RemoveColdEffects();
            aZombie->ApplyBurn();
            continue;
        }

        Rect aZombieRect = aZombie->GetZombieRect();
        if (GetRectOverlap(aColumnRect, aZombieRect) > 0 && aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            aZombie->RemoveColdEffects();
            aZombie->ApplyBurn();
        }
    }

    GridItem* aGridItem = nullptr;
    while (mBoard->IterateGridItems(aGridItem))
    {
        if (aGridItem->mGridX == theCol && aGridItem->mGridItemType == GridItemType::GRIDITEM_LADDER)
        {
            aGridItem->GridItemDie();
        }
    }
}

void Plant::BlowAwayFliers(int theX, int theRow)
{
    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (!aZombie->IsDeadOrDying())
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            if (aZombie->IsFlying())
            {
                aZombie->mBlowingAway = true;
            }
        }
    }

    mApp->PlaySample(SOUND_BLOVER);
    mBoard->mFogBlownCountDown = 4000;
}

void Plant::KillAllPlantsNearDoom()
{
    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (aPlant->mRow == mRow && aPlant->mPlantCol == mPlantCol)
        {
            aPlant->Die();
        }
    }
}

void Plant::DoSpecial()
{
    int aPosX = mX + mWidth / 2;
    int aPosY = mY + mHeight / 2;
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);

    switch (mSeedType)
    {
    case SeedType::SEED_BLOVER:
    {
        if (mState != PlantState::STATE_DOINGSPECIAL)
        {
            mState = PlantState::STATE_DOINGSPECIAL;
            BlowAwayFliers(mX, mRow); // This still clears fog and pops balloons

            int aPushDuration = 150; // The push will last for 150 ticks (1.5 Second)

            Zombie* aZombie = nullptr;
            while (mBoard->IterateZombies(aZombie))
            {
                // Skip zombies that are dead, off-screen, or the boss
                if (aZombie->IsDeadOrDying() || 
                    !aZombie->IsOnBoard() || 
                    aZombie->mMindControlled ||
                    aZombie->mZombieType == ZOMBIE_BOSS ||
                    aZombie->mZombieType == ZOMBIE_BUNGEE ||
                    aZombie->mZombieType == ZOMBIE_YETI)
                {
                    continue;
                }

                // Stop the zombie from eating while being pushed
                aZombie->StopEating();

                if (aZombie->mRow == mRow)
                {
                    // For zombies in the Blover's own lane:
                    float aTargetX = mBoard->GridToPixelX(8, mRow) + 40.0f;
                    float aDistance = aTargetX - aZombie->mPosX;
                    aZombie->mPushX = aDistance / aPushDuration; // Calculate speed needed to reach the back
                    aZombie->mPushbackCount = aPushDuration;
                    aZombie->ApplyChill(false);
                }
                else
                {
                    // For zombies in all other lanes:
                    aZombie->mPushX = 240.0f / aPushDuration; // 3 blocks (240px) over the duration
                    aZombie->mPushbackCount = aPushDuration;
                }
            }
        }
        break;
    }
    case SeedType::SEED_CHERRYBOMB:
    {
        mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB);
        mApp->PlayFoley(FoleyType::FOLEY_JUICY);

        if (mBoard->GetAllZombiesInRadius(mRow, aPosX, aPosY, 115, 1, aDamageRangeFlags) >= 10 && !mApp->mPlayedQuickplay)
        {
            mApp->GetAchievement(ACHIEVEMENT_EXPLODONATOR);
        }
        mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 115, 1, true, aDamageRangeFlags);


        mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POWIE);
        mBoard->ShakeBoard(3, -4);

        float aProjectileSpeed = 10.0f;
        int aNumProjectiles = 8;
        for (int i = 0; i < aNumProjectiles; i++)
        {
            float anAngle = DEG_TO_RAD(360.0f * i / aNumProjectiles);

            Projectile* aProjectile = mBoard->AddProjectile(aPosX - 40, aPosY - 40, mRenderOrder, mRow, ProjectileType::PROJECTILE_GRAPESHOT);

            if (aProjectile)
            {
                aProjectile->mMotionType = ProjectileMotion::MOTION_BOUNCE;
                aProjectile->mVelX = cos(anAngle) * aProjectileSpeed;
                aProjectile->mVelY = sin(anAngle) * aProjectileSpeed;
                aProjectile->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
            }
        }

        Die();
        break;
    }
    case SeedType::SEED_DOOMSHROOM:
    {
        mApp->PlaySample(SOUND_DOOMSHROOM);

        int aDamage = 900; // 50% of the usual 1800 instant-kill damage
        int aRadius = 250;
        unsigned int aDamageFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);

        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if (aZombie->EffectedByDamage(aDamageFlags))
            {
                Rect aZombieRect = aZombie->GetZombieRect();
                if (GetCircleRectOverlap(aPosX, aPosY, aRadius, aZombieRect))
                {
                    aZombie->TakeDamage(aDamage, 0U); // Deal initial 50% damage
                    if (!aZombie->IsDeadOrDying())
                    {
                        aZombie->mIsRadioactive = true; // Apply radioactive state to survivors
                        aZombie->mRadioactiveCountdown = 100;

                    }
                }
            }
        }

        bool aHasFlowerPot = (mBoard->GetFlowerPotAt(mPlantCol, mRow) != nullptr);

        KillAllPlantsNearDoom();

        mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_DOOM);
        if (!aHasFlowerPot)
        {
            mBoard->AddACrater(mPlantCol, mRow)->mGridItemCounter = 18000;
        }
        mBoard->ShakeBoard(3, -4);

        Die();
        break;
    }
    case SeedType::SEED_JALAPENO:
    {
        mApp->PlayFoley(FoleyType::FOLEY_JALAPENO_IGNITE);
        mApp->PlayFoley(FoleyType::FOLEY_JUICY);

        mBoard->DoFwoosh(mRow);
        mBoard->DoFwooshColumn(mPlantCol);
        mBoard->ShakeBoard(3, -4);

        BurnRow(mRow);
        BurnColumn(mPlantCol);
        mBoard->mIceTimer[mRow] = 20;

        Die();
        break;
    }
    case SeedType::SEED_PICKLEDPEPPER:
    {
        mApp->PlayFoley(FOLEY_JALAPENO_IGNITE);
        mBoard->ShakeBoard(3, -4);

        // Define the 3x5 area, centered on the plant
        int aCenterRow = mRow;
        int aCenterCol = mPlantCol;

        for (int i = -2; i <= 2; i++) // 5 columns wide
        {
            for (int j = -1; j <= 1; j++) // 3 rows high
            {
                int aTargetCol = aCenterCol + i;
                int aTargetRow = aCenterRow + j;

                // Make sure the target tile is on the board
                if (aTargetCol >= 0 && aTargetCol < MAX_GRID_SIZE_X && aTargetRow >= 0 && aTargetRow < MAX_GRID_SIZE_Y)
                {
                    mBoard->AddFirePlace(aTargetCol, aTargetRow);
                }
            }
        }

        Die(); // The plant disappears after exploding
        break;
    }

    case SeedType::SEED_ABSOLUTELEAF:
    case SeedType::SEED_UMBRELLA:
    {
        if (mState != PlantState::STATE_UMBRELLA_TRIGGERED && mState != PlantState::STATE_UMBRELLA_REFLECTING)
        {
            mState = PlantState::STATE_UMBRELLA_TRIGGERED;
            mStateCountdown = 5;

            PlayBodyReanim("anim_block", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 22.0f);
        }

        break;
    }
    case SeedType::SEED_ICESHROOM:
    {
        mApp->PlayFoley(FoleyType::FOLEY_FROZEN);
        IceZombies();
        mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_ICE_TRAP);

        Die();
        break;
    }
    case SeedType::SEED_POTATOMINE:
    {
        aPosX = mX + mWidth / 2 - 20;
        aPosY = mY + mHeight / 2;

        mApp->PlaySample(SOUND_POTATO_MINE);
        mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 60, 0, false, aDamageRangeFlags);
        if(!mApp->IsIZombieLevel() && !mApp->mPlayedQuickplay)
            mApp->GetAchievement(AchievementType::ACHIEVEMENT_SPUDOW);

        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
        mApp->AddTodParticle(aPosX + 20.0f, aPosY, aRenderPosition, ParticleEffect::PARTICLE_POTATO_MINE);
        mBoard->ShakeBoard(3, -4);

        Die();
        break;
    }
    case SEED_RED_POTATO_MINE: // <-- ADD THIS NEW CASE BLOCK
    {
        float aPosX = mX + 40;
        float aPosY = mY + 40;

        mApp->PlaySample(SOUND_POTATO_MINE);
        mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 115, 1, true, aDamageRangeFlags);
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
        mApp->AddTodParticle(aPosX + 20.0f, aPosY, aRenderPosition, ParticleEffect::PARTICLE_POTATO_MINE);
        mBoard->ShakeBoard(3, -4);

        int aDamageRangeFlags = 0;
        SetBit(reinterpret_cast<unsigned int&>(aDamageRangeFlags), DAMAGE_DOESNT_LEAVE_BODY, true);
        SetBit(reinterpret_cast<unsigned int&>(aDamageRangeFlags), DAMAGE_FIRE, true);

        Die();
        break;
    }
    case SeedType::SEED_INSTANT_COFFEE:
    {
        Plant* aPlant = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
        if (aPlant) // Check if there's a plant to affect at all
        {
            if (aPlant->mIsAsleep)
            {
                aPlant->mWakeUpCounter = 100;
            }
            else if (aPlant->mChilledCounter > 0)
            {
                aPlant->mChilledCounter = 0; // Remove chill
                mApp->PlayFoley(FoleyType::FOLEY_WAKEUP); // Play a sound

                aPlant->PlayIdleAnim(aPlant->mBodyReanimID != REANIMATIONID_NULL ? mApp->ReanimationGet(aPlant->mBodyReanimID)->mDefinition->mFPS : 0.0f);
            }
            else // If the plant is already awake, boost it
            {
                aPlant->mIsBoosted = true;
                aPlant->mBoostCounter = 500; // 5 seconds (500 ticks)
                aPlant->mLaunchCounter = 0;
            }
        }

        mState = PlantState::STATE_DOINGSPECIAL;
        PlayBodyReanim("anim_crumble", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 22.0f);
        mApp->PlayFoley(FoleyType::FOLEY_COFFEE);

        break;
    }
    }
}

void Plant::ImitaterMorph()
{
    Die();
    Plant* aPlant = mBoard->AddPlant(mPlantCol, mRow, mImitaterType, SeedType::SEED_IMITATER);

    FilterEffect aFilter = FilterEffect::FILTER_EFFECT_WASHED_OUT;
    if (mImitaterType == SeedType::SEED_HYPNOSHROOM || mImitaterType == SeedType::SEED_SQUASH || mImitaterType == SeedType::SEED_POTATOMINE ||
        mImitaterType == SeedType::SEED_GARLIC || mImitaterType == SeedType::SEED_LILYPAD)
        aFilter = FilterEffect::FILTER_EFFECT_LESS_WASHED_OUT;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(aPlant->mBodyReanimID);
    if (aBodyReanim)
    {
        aBodyReanim->mFilterEffect = aFilter;
    }
    Reanimation* aHeadReanim = mApp->ReanimationTryToGet(aPlant->mHeadReanimID);
    if (aHeadReanim)
    {
        aHeadReanim->mFilterEffect = aFilter;
    }Reanimation* aHeadReanim2 = mApp->ReanimationTryToGet(aPlant->mHeadReanimID2);
    if (aHeadReanim2)
    {
        aHeadReanim2->mFilterEffect = aFilter;
    }Reanimation* aHeadReanim3 = mApp->ReanimationTryToGet(aPlant->mHeadReanimID3);
    if (aHeadReanim3)
    {
        aHeadReanim3->mFilterEffect = aFilter;
    }
}

void Plant::UpdateImitater()
{
    if (mState != PlantState::STATE_IMITATER_MORPHING)
    {
        if (mStateCountdown == 0)
        {
            mState = PlantState::STATE_IMITATER_MORPHING;
            PlayBodyReanim("anim_explode", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 26.0f);
        }
    }
    else
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->ShouldTriggerTimedEvent(0.8f))
        {
            mApp->AddTodParticle(mX + 40, mY + 40, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_IMITATER_MORPH);
        }
        if (aBodyReanim->mLoopCount > 0)
        {
            ImitaterMorph();
        }
    }
}

void Plant::CobCannonFire(int theTargetX, int theTargetY)
{
    TOD_ASSERT(mState == PlantState::STATE_COBCANNON_READY);

    mState = PlantState::STATE_COBCANNON_FIRING;
    mShootingCounter = 206;
    PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);

    mTargetX = theTargetX - 47.0f;
    mTargetY = theTargetY;

    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("CobCannon_Cob");
    aTrackInstance->mTrackColor = Color::White;
}

void Plant::Fire(Zombie* theTargetZombie, int theRow, PlantWeapon thePlantWeapon)
{
    if (mSeedType == SeedType::SEED_FUMESHROOM)
    {
        DoRowAreaDamage(20, 2U);
        mApp->PlayFoley(FoleyType::FOLEY_FUME);
        return;
    }
    if (mSeedType == SeedType::SEED_GLOOMSHROOM)
    {
        DoRowAreaDamage(20, 2U);

        mGloomShotCount++;
        if (mGloomShotCount >= 4)
        {
            mGloomShotCount = 0;
            mGloomMode = (GloomMode)((mGloomMode + 1) % 4); // Cycle to the next mode
            mApp->PlayFoley(FOLEY_BLEEP); // Play a sound to indicate a mode change
        }
        return;
    }
    if (mSeedType == SeedType::SEED_STARFRUIT)
    {
        StarFruitFire();
        return;
    }
    else if (mSeedType == SeedType::SEED_SOUR_STARFRUIT)
    {
        FireSourStarfruit();
        return;
    }

    ProjectileType aProjectileType;
    switch (mSeedType)
    {
    case SeedType::SEED_PEASHOOTER:
        if (Rand(100) < 20)
        {
            aProjectileType = ProjectileType::PROJECTILE_BIG_PEA;
        }
        else
        {
            aProjectileType = ProjectileType::PROJECTILE_PEA;
        }
        break;
    case SeedType::SEED_THREEPEATER:
    case SeedType::SEED_LEFTPEATER:
    case SeedType::SEED_BLACK_THREEPEATER:
        aProjectileType = ProjectileType::PROJECTILE_PEA;
        break;

    case SeedType::SEED_GATLINGPEA:
    {
        int aChance = Rand(100); // A random number from 0 to 99
        if (aChance < 10) // 10% chance
        {
            aProjectileType = ProjectileType::PROJECTILE_RED_FIRE_PEA;
            mApp->PlayFoley(FoleyType::FOLEY_FIREPEA);
        }
        else if (aChance < 50) // 40% chance (10% + 40% = 50%)
        {
            aProjectileType = ProjectileType::PROJECTILE_FIREBALL;
            mApp->PlayFoley(FoleyType::FOLEY_FIREPEA);
        }
        else // Remaining 50% chance
        {
            aProjectileType = ProjectileType::PROJECTILE_PEA;
            mApp->PlayFoley(FoleyType::FOLEY_THROW);
        }
        break;
    }
    case SeedType::SEED_FIRESHOOTER:
    {
        int aRand = Rand(100);
        if (aRand < 1)
        {
            aProjectileType = ProjectileType::PROJECTILE_WHITE_FIRE_PEA;
            mApp->PlayFoley(FoleyType::FOLEY_FIREPEA);
        }
        else if (aRand < 6)
        {
            aProjectileType = ProjectileType::PROJECTILE_BLUE_FIRE_PEA;
            mApp->PlayFoley(FoleyType::FOLEY_FROZEN);
        }
        else if (aRand < 26)
        {
            aProjectileType = ProjectileType::PROJECTILE_RED_FIRE_PEA;
            mApp->PlayFoley(FoleyType::FOLEY_FIREPEA);
        }
        else if (aRand < 56)
        {
            aProjectileType = ProjectileType::PROJECTILE_BIG_FIREPEA;
            mApp->PlayFoley(FoleyType::FOLEY_FIREPEA);
        }
        else 
        {
            aProjectileType = ProjectileType::PROJECTILE_FIREBALL;
            mApp->PlayFoley(FoleyType::FOLEY_FIREPEA);
        }
        
        break;
    }
    case SeedType::SEED_SPLITPEA:
    case SeedType::SEED_SUPER_SPLITPEA:
        if (thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
        {
            int aChance = Rand(100);
            if (aChance < 15)
                aProjectileType = ProjectileType::PROJECTILE_FIREBALL;
            else
                aProjectileType = ProjectileType::PROJECTILE_BOUNCING_PEA;
        }
        else
        {
            int aChance = Rand(100);
            if (aChance < 5)
                aProjectileType = ProjectileType::PROJECTILE_RED_FIRE_PEA;
            else if (aChance < 25)
                aProjectileType = ProjectileType::PROJECTILE_FIREBALL;
            else
                aProjectileType = ProjectileType::PROJECTILE_PEA;
        }
        break;
    case SeedType::SEED_REPEATER:
    {
        int aChance = Rand(100);
        if (aChance < 1) // 1% chance for Blue Fire Pea
        {
            aProjectileType = ProjectileType::PROJECTILE_BLUE_FIRE_PEA;
            mApp->PlayFoley(FoleyType::FOLEY_FROZEN);
        }
        else if (aChance < 6) // 5% chance for Red Fire Pea (1 + 5)
        {
            aProjectileType = ProjectileType::PROJECTILE_RED_FIRE_PEA;
            mApp->PlayFoley(FoleyType::FOLEY_FIREPEA);
        }
        else if (aChance < 36) // 30% chance for Fireball (6 + 30)
        {
            aProjectileType = ProjectileType::PROJECTILE_FIREBALL;
            mApp->PlayFoley(FoleyType::FOLEY_FIREPEA);
        }
        else // Remaining 64% chance for Normal Pea
        {
            aProjectileType = ProjectileType::PROJECTILE_PEA;
            mApp->PlayFoley(FoleyType::FOLEY_THROW);
        }
        break;
    }

    case SeedType::SEED_COMMANDOPEA:
    case SeedType::SEED_GENERALPEA:
    {
        aProjectileType = ProjectileType::PROJECTILE_PEA_SNIPE;
        break;
    }
    case SeedType::SEED_CHERRYBOMB:
        aProjectileType = ProjectileType::PROJECTILE_GRAPESHOT;
        break;
    case SeedType::SEED_SNOWPEA:
        if (Rand(100) < 15)
        {
            aProjectileType = ProjectileType::PROJECTILE_ICEPEA;
            mApp->PlayFoley(FoleyType::FOLEY_FROZEN);
        }
        else
        {
            aProjectileType = ProjectileType::PROJECTILE_SNOWPEA;
        }
        break;
    case SeedType::SEED_ICEPEA:
        aProjectileType = ProjectileType::PROJECTILE_ICEPEA;
        break;
    case SeedType::SEED_PUFFSHROOM:
    {
        ProjectileType aCurrentProjectileType = ProjectileType::PROJECTILE_PUFF;
        if (Rand(100) < 5)
        {
            aCurrentProjectileType = ProjectileType::PROJECTILE_BUTTER;
            mApp->PlayFoley(FoleyType::FOLEY_BUTTER);
        }
        else
        {
            mApp->PlayFoley(FoleyType::FOLEY_PUFF);
        }

        int aOriginX = mX + 40;
        int aOriginY = mY + 40;

        Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder - 1, theRow, aCurrentProjectileType);
        aProjectile->mDamageRangeFlags = GetDamageRangeFlags(thePlantWeapon);

        aProjectile->mMotionType = ProjectileMotion::MOTION_PUFF;
        return;
    }
    case SeedType::SEED_SCAREDYSHROOM:
    {
        // If the plant is in random mode
        if (mIsRandom)
        {
            // Check if the player has enough sun
            if (!mBoard->CanTakeSunMoney(2))
            {
                mApp->PlaySample(SOUND_BUZZER);
                return; // Not enough sun, don't fire
            }
            mBoard->TakeSunMoney(2); // Subtract 1 sun

            // Define the possible projectiles and their chances
            TodWeightedArray aProjectileArray[] = {
                { (int)ProjectileType::PROJECTILE_PEA,                  10 },
                { (int)ProjectileType::PROJECTILE_SNOWPEA,              10 },
                { (int)ProjectileType::PROJECTILE_ICEPEA,               10 },
                { (int)ProjectileType::PROJECTILE_CABBAGE,              10 },
                { (int)ProjectileType::PROJECTILE_KERNEL,               10 },
                { (int)ProjectileType::PROJECTILE_BUTTER,               10 },
                { (int)ProjectileType::PROJECTILE_FIREBALL,             10 },
                { (int)ProjectileType::PROJECTILE_PUFF,                 10 },
                { (int)ProjectileType::PROJECTILE_POISON_CABBAGE,       10 }
            };
            int aProjectileCount = sizeof(aProjectileArray) / sizeof(aProjectileArray[0]);
            ProjectileType aRandomProjectile = (ProjectileType)TodPickFromWeightedArray(aProjectileArray, aProjectileCount);

            // Create the projectile
            int aOriginX = mX + 29;
            int aOriginY = mY + 21;
            Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder - 1, theRow, aRandomProjectile);
            aProjectile->mDamageRangeFlags = GetDamageRangeFlags(thePlantWeapon);
            aProjectile->mMotionType = ProjectileMotion::MOTION_STRAIGHT;
            mApp->PlayFoley(FOLEY_PUFF);
        }
        else // If not in random mode, fire a normal puff
        {
            int aOriginX = mX + 29;
            int aOriginY = mY + 21;
            Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder - 1, theRow, ProjectileType::PROJECTILE_PUFF);
            aProjectile->mDamageRangeFlags = GetDamageRangeFlags(thePlantWeapon);
            aProjectile->mMotionType = ProjectileMotion::MOTION_STRAIGHT;
            mApp->PlayFoley(FOLEY_PUFF);
        }
        return; // Important: Exit here to avoid running the default code below
    }
    case SeedType::SEED_SEASHROOM:
        if (Rand(100) < 5) // 5% chance
        {
            aProjectileType = ProjectileType::PROJECTILE_BUTTER;
            mApp->PlayFoley(FoleyType::FOLEY_BUTTER);
        }
        else // 95% chance
        {
            aProjectileType = ProjectileType::PROJECTILE_PUFF;
            mApp->PlayFoley(FoleyType::FOLEY_PUFF);
        }
        break;
    case SeedType::SEED_CACTUS:
    case SeedType::SEED_CATTAIL:
        if (Rand(100) < 5) // 5% chance for Black Spike
        {
            aProjectileType = ProjectileType::PROJECTILE_BLACK_SPIKE;
        }
        else if (mSeedType == SEED_CACTUS && Rand(100) < 50) // 50% of the remaining shots are Blue Spikes
        {
            aProjectileType = ProjectileType::PROJECTILE_BLUE_SPIKE;
        }
        else // Otherwise, fire a normal spike
        {
            aProjectileType = ProjectileType::PROJECTILE_SPIKE;
        }
        break;
    case SeedType::SEED_MAD_CACTUS:
        if (mSeedType == SEED_CACTUS && Rand(100) < 50) // 50% of the remaining shots are Blue Spikes
        {
            aProjectileType = ProjectileType::PROJECTILE_BLUE_SPIKE;
        }
        else
        {
            aProjectileType = ProjectileType::PROJECTILE_BLACK_SPIKE;
        }
        break;
    case SeedType::SEED_CABBAGEPULT:
        if (Rand(4) == 0) // 1 in 4 chance (25%)
        {
            aProjectileType = ProjectileType::PROJECTILE_POISON_CABBAGE;
        }
        else
        {
            aProjectileType = ProjectileType::PROJECTILE_CABBAGE;
        }
        break;
    case SeedType::SEED_POISONPULT:
        if (Rand(4) == 0) // 1 in 4 chance (25%)
        {
            aProjectileType = ProjectileType::PROJECTILE_BIG_POISON_CABBAGE;
        }
        else
        {
            aProjectileType = ProjectileType::PROJECTILE_POISON_CABBAGE;
        }
        break;
    case SeedType::SEED_KERNELPULT:
        aProjectileType = ProjectileType::PROJECTILE_KERNEL;
        break;
    case SeedType::SEED_MELONPULT:
        aProjectileType = ProjectileType::PROJECTILE_MELON;
        break;
    case SeedType::SEED_WINTERMELON:
        aProjectileType = ProjectileType::PROJECTILE_WINTERMELON;
        break;
    case SeedType::SEED_COBCANNON:
        aProjectileType = ProjectileType::PROJECTILE_COBBIG;
        break;
    case SeedType::SEED_HATTREMWITCH:
    case SeedType::SEED_HATTREMSAGE:
        aProjectileType = ProjectileType::PROJECTILE_BIG_POISON_CABBAGE;
        break;
    default:
        TOD_ASSERT();
        break;
    }
    int aOriginX, aOriginY;
    if (mSeedType == SeedType::SEED_KERNELPULT && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
    {
        if (mState == PlantState::STATE_KERNELPULT_BIG_BUTTER)
        {
            aProjectileType = ProjectileType::PROJECTILE_BIG_BUTTER;
        }
        else
        {
            aProjectileType = ProjectileType::PROJECTILE_BUTTER;
        }
    }

    if (mSeedType != SEED_GATLINGPEA)
    {
        mApp->PlayFoley(FoleyType::FOLEY_THROW);
    }
    if (mSeedType == SeedType::SEED_SNOWPEA || mSeedType == SeedType::SEED_WINTERMELON || mSeedType == SeedType::SEED_ICEPEA)
    {
        mApp->PlayFoley(FoleyType::FOLEY_SNOW_PEA_SPARKLES);
    }
    else if (mSeedType == SeedType::SEED_PUFFSHROOM || mSeedType == SeedType::SEED_SCAREDYSHROOM || mSeedType == SeedType::SEED_SEASHROOM)
    {
        mApp->PlayFoley(FoleyType::FOLEY_PUFF);
    }

    
    if (mSeedType == SeedType::SEED_PUFFSHROOM)
    {
        aOriginX = mX + 40;
        aOriginY = mY + 40;
    }
    else if (mSeedType == SeedType::SEED_SEASHROOM)
    {
        aOriginX = mX + 45;
        aOriginY = mY + 63;
    }
    else if (mSeedType == SeedType::SEED_CABBAGEPULT || mSeedType == SeedType::SEED_POISONPULT)
    {
        aOriginX = mX + 5;
        aOriginY = mY - 12;
    }
    else if (mSeedType == SeedType::SEED_MELONPULT || mSeedType == SeedType::SEED_WINTERMELON)
    {
        aOriginX = mX + 25;
        aOriginY = mY - 46;
    }
    else if (mSeedType == SeedType::SEED_CATTAIL)
    {
        aOriginX = mX + 20;
        aOriginY = mY - 3;
    }
    else if (mSeedType == SeedType::SEED_KERNELPULT && thePlantWeapon == PlantWeapon::WEAPON_PRIMARY)
    {
        aOriginX = mX + 19;
        aOriginY = mY - 37;
    }
    else if (mSeedType == SeedType::SEED_KERNELPULT && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
    {
        aOriginX = mX + 12;
        aOriginY = mY - 56;
    }
    else if (mSeedType == SeedType::SEED_PEASHOOTER || mSeedType == SeedType::SEED_SNOWPEA || mSeedType == SeedType::SEED_REPEATER || 
        mSeedType == SeedType::SEED_FIRESHOOTER || mSeedType == SeedType::SEED_ICEPEA || mSeedType == SeedType::SEED_COMMANDOPEA ||
        mSeedType == SeedType::SEED_GENERALPEA)
    {
        int aOffsetX, aOffsetY;
        GetPeaHeadOffset(aOffsetX, aOffsetY);
        aOriginX = mX + aOffsetX + 24;
        aOriginY = mY + aOffsetY - 33;
    }
    else if (mSeedType == SeedType::SEED_LEFTPEATER)
    {
        int aOffsetX, aOffsetY;
        GetPeaHeadOffset(aOffsetX, aOffsetY);
        aOriginX = mX + aOffsetX - 30;
        aOriginY = mY + aOffsetY - 33;
    }
    else if (mSeedType == SeedType::SEED_GATLINGPEA)
    {
        int aOffsetX, aOffsetY;
        GetPeaHeadOffset(aOffsetX, aOffsetY);
        aOriginX = mX + aOffsetX + 34;
        aOriginY = mY + aOffsetY - 33;
    }
    else if (mSeedType == SeedType::SEED_SPLITPEA || mSeedType == SeedType::SEED_SUPER_SPLITPEA)
    {
        int aOffsetX, aOffsetY;
        GetPeaHeadOffset(aOffsetX, aOffsetY);
        aOriginY = mY + aOffsetY - 33;

        if (thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
        {
            aOriginX = mX + aOffsetX - 64;
        }
        else
        {
            aOriginX = mX + aOffsetX + 24;
        }
    }
    else if (mSeedType == SeedType::SEED_THREEPEATER || mSeedType == SeedType::SEED_BLACK_THREEPEATER)
    {
        aOriginX = mX + 45;
        aOriginY = mY + 10;
    }
    else if (mSeedType == SeedType::SEED_SCAREDYSHROOM)
    {
        aOriginX = mX + 29;
        aOriginY = mY + 21;
    }
    else if (mSeedType == SeedType::SEED_CACTUS || mSeedType == SeedType::SEED_MAD_CACTUS)
    {
        if (thePlantWeapon == PlantWeapon::WEAPON_PRIMARY)
        {
            aOriginX = mX + 93;
            aOriginY = mY - 50;
        }
        else
        {
            aOriginX = mX + 70;
            aOriginY = mY + 23;
        }
    }
    else if (mSeedType == SeedType::SEED_COBCANNON)
    {
        aOriginX = mX - 44;
        aOriginY = mY - 184;
    }
    else
    {
        aOriginX = mX + 10;
        aOriginY = mY + 5;
    }
    if (mBoard->GetFlowerPotAt(mPlantCol, mRow))
    {
        aOriginY -= 5;
    }

    if (mSeedType == SeedType::SEED_SNOWPEA || mSeedType == SeedType::SEED_ICEPEA)
    {
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_LAWN_MOWER, mRow, 1);
        mApp->AddTodParticle(aOriginX + 8, aOriginY + 13, aRenderPosition, ParticleEffect::PARTICLE_SNOWPEA_PUFF);
    }
    else if (mSeedType == SeedType::SEED_PUFFSHROOM)
    {
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_LAWN_MOWER, mRow, 1);
        mApp->AddTodParticle(aOriginX + 18, aOriginY + 13, aRenderPosition, ParticleEffect::PARTICLE_PUFFSHROOM_MUZZLE);
    }
    else if (mSeedType == SeedType::SEED_SCAREDYSHROOM)
    {
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_LAWN_MOWER, mRow, 1);
        mApp->AddTodParticle(aOriginX + 27, aOriginY + 13, aRenderPosition, ParticleEffect::PARTICLE_PUFFSHROOM_MUZZLE);
    }

    const PlantDefinition& aPlantDef = GetPlantDefinition(mSeedType);
    int aNumProjectiles = 1; // Each burst tick fires 1 projectile matching 1 animation frame

    if (aProjectileType == PROJECTILE_BIG_FIREPEA)
    {
        aOriginY -= 35;
        mApp->PlayFoley(FoleyType::FOLEY_FIREPEA);
    }

    for (int i = 0; i < aNumProjectiles; i++)
    {
        int aProjectileXOffset = i * 60;

        int aProjectileY = aOriginY + (i > 0 ? RandRangeInt(-5, 5) : 0);

        ProjectileType aCurrentProjectileType = aProjectileType;
        if (mSeedType == SeedType::SEED_SNOWPEA && Rand(100) < 5)
        {
            aCurrentProjectileType = ProjectileType::PROJECTILE_ICEPEA;
        }

        Projectile* aProjectile = mBoard->AddProjectile(aOriginX + aProjectileXOffset, aOriginY, mRenderOrder - 1, theRow, aCurrentProjectileType);
        if (aProjectile->mProjectileType == PROJECTILE_BLACK_SPIKE)
        {
            aProjectile->mDamageRangeFlags = (1 << DAMAGES_GROUND) | (1 << DAMAGES_FLYING);
        }
        else
        {
            aProjectile->mDamageRangeFlags = GetDamageRangeFlags(thePlantWeapon);
        }

        if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_FIREBALL ||
            aProjectile->mProjectileType == ProjectileType::PROJECTILE_RED_FIRE_PEA ||
            aProjectile->mProjectileType == ProjectileType::PROJECTILE_BLUE_FIRE_PEA ||
            aProjectile->mProjectileType == ProjectileType::PROJECTILE_WHITE_FIRE_PEA ||
            aProjectile->mProjectileType == ProjectileType::PROJECTILE_BIG_FIREPEA)
        {
            if (aProjectile->mMotionType == ProjectileMotion::MOTION_BACKWARDS)
            {
                Reanimation* aFirePeaReanim = FindReanimAttachment(aProjectile->mAttachmentID);
                if (aFirePeaReanim)
                {
                    aFirePeaReanim->OverrideScale(-1.0f, 1.0f);
                }
            }
        }
        aProjectile->mDamageRangeFlags = GetDamageRangeFlags(thePlantWeapon);

        if (mSeedType == SeedType::SEED_CABBAGEPULT || mSeedType == SeedType::SEED_KERNELPULT ||
            mSeedType == SeedType::SEED_MELONPULT || mSeedType == SeedType::SEED_WINTERMELON ||
            mSeedType == SeedType::SEED_POISONPULT)
        {
            float aRangeX, aRangeY;
            if (theTargetZombie)
            {
                Rect aZombieRect = theTargetZombie->GetZombieRect();
                aRangeX = theTargetZombie->ZombieTargetLeadX(50.0f) - aOriginX - 30.0f;
                aRangeY = aZombieRect.mY - aOriginY;

                if (theTargetZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_RIDING)
                {
                    aRangeX -= 60.0f;
                }
                if (theTargetZombie->mZombieType == ZombieType::ZOMBIE_POGO && theTargetZombie->mHasObject)
                {
                    aRangeX -= 60.0f;
                }
                if (theTargetZombie->mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL)
                {
                    aRangeX -= 40.0f;
                }
                if (theTargetZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
                {
                    aRangeY = mBoard->GridToPixelY(8, mRow) - aOriginY;
                }
            }
            else
            {
                aRangeX = 700.0f - aOriginX;
                aRangeY = 0.0f;
            }
            if (aRangeX < 40.0f)
            {
                aRangeX = 40.0f;
            }

            aProjectile->mMotionType = ProjectileMotion::MOTION_LOBBED;
            aProjectile->mVelX = aRangeX / 120.0f;
            aProjectile->mVelY = 0.0f;
            aProjectile->mVelZ = aRangeY / 120.0f - 7.0f;
            aProjectile->mAccZ = 0.115f;
        }
        else if (mSeedType == SeedType::SEED_THREEPEATER || mSeedType == SeedType::SEED_BLACK_THREEPEATER)
        {
            if (theRow < mRow)
            {
                aProjectile->mMotionType = ProjectileMotion::MOTION_THREEPEATER;
                aProjectile->mVelY = -3.0f;
                aProjectile->mShadowY += 80.0f;
            }
            else if (theRow > mRow)
            {
                aProjectile->mMotionType = ProjectileMotion::MOTION_THREEPEATER;
                aProjectile->mVelY = 3.0f;
                aProjectile->mShadowY -= 80.0f;
            }
        }
        else if (mSeedType == SeedType::SEED_PUFFSHROOM || mSeedType == SeedType::SEED_SEASHROOM)
        {
            aProjectile->mMotionType = ProjectileMotion::MOTION_PUFF;
        }
        else if ((mSeedType == SeedType::SEED_SPLITPEA || mSeedType == SeedType::SEED_SUPER_SPLITPEA) && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
        {
            aProjectile->mMotionType = ProjectileMotion::MOTION_BACKWARDS;
        }
        else if (mSeedType == SeedType::SEED_LEFTPEATER)
        {
            aProjectile->mMotionType = ProjectileMotion::MOTION_BACKWARDS;
        }
        else if (mSeedType == SeedType::SEED_CATTAIL)
        {
            aProjectile->mVelX = 2.0f;
            aProjectile->mMotionType = ProjectileMotion::MOTION_HOMING;
            aProjectile->mTargetZombieID = mBoard->ZombieGetID(theTargetZombie);
        }
        else if (mSeedType == SeedType::SEED_COMMANDOPEA || mSeedType == SeedType::SEED_GENERALPEA)
        {
            float aSpeed = 16.0f;
            if (theTargetZombie != nullptr)
            {
                Rect aZombieRect = theTargetZombie->GetZombieRect();
                float aTargetX = (float)theTargetZombie->ZombieTargetLeadX(0.0f);
                float aTargetY = (float)(aZombieRect.mY + aZombieRect.mHeight / 2);
                float aDx = aTargetX - (float)aOriginX;
                float aDy = aTargetY - (float)aOriginY;
                float aDist = sqrtf(aDx * aDx + aDy * aDy);
                if (aDist > 1.0f)
                {
                    aProjectile->mVelX = (aDx / aDist) * aSpeed;
                    aProjectile->mVelY = (aDy / aDist) * aSpeed;
                }
                else
                {
                    aProjectile->mVelX = aSpeed;
                    aProjectile->mVelY = 0.0f;
                }
            }
            else
            {
                aProjectile->mVelX = aSpeed;
                aProjectile->mVelY = 0.0f;
            }
            aProjectile->mMotionType = ProjectileMotion::MOTION_STRAIGHT;
            aProjectile->mRotation = -atan2f(aProjectile->mVelY, aProjectile->mVelX);
        }
        else if (mSeedType == SeedType::SEED_COBCANNON)
        {
            aProjectile->mVelX = 0.001f;
            aProjectile->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
            aProjectile->mMotionType = ProjectileMotion::MOTION_LOBBED;
            aProjectile->mVelY = 0.0f;
            aProjectile->mAccZ = 0.0f;
            aProjectile->mVelZ = -8.0f;
            aProjectile->mCobTargetX = mTargetX - 40;
            aProjectile->mCobTargetRow = mBoard->PixelToGridYKeepOnBoard(mTargetX, mTargetY);
        }
    }
}

Zombie* Plant::FindTargetZombie(int theRow, PlantWeapon thePlantWeapon)
{
    if (mSeedType == SeedType::SEED_STARFRUIT || mSeedType == SeedType::SEED_SOUR_STARFRUIT)
    {
        // Starfruit fires in 5 directions including left and diagonal - use its own targeting logic
        if (FindStarFruitTarget())
        {
            // Return any zombie just so the caller knows there IS a target; 
            // StarFruitFire() handles actual firing direction itself
            Zombie* aAnyZombie = nullptr;
            mBoard->IterateZombies(aAnyZombie);
            return aAnyZombie;
        }
        return nullptr;
    }

    if ((mSeedType == SeedType::SEED_SPLITPEA || mSeedType == SeedType::SEED_SUPER_SPLITPEA) && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
    {
        Rect aBackRect = GetPlantAttackRect(PlantWeapon::WEAPON_SECONDARY);
        Zombie* aZombieBehind = nullptr;
        int aMaxX = -1;

        Zombie* aZombieIter = nullptr;
        while (mBoard->IterateZombies(aZombieIter))
        {
            if (aZombieIter->mRow == theRow && aZombieIter->EffectedByDamage(GetDamageRangeFlags(thePlantWeapon)))
            {
                Rect aZombieRect = aZombieIter->GetZombieRect();
                if (GetRectOverlap(aBackRect, aZombieRect) > 0)
                {
                    if (aZombieBehind == nullptr || aZombieRect.mX > aMaxX)
                    {
                        aZombieBehind = aZombieIter;
                        aMaxX = aZombieRect.mX;
                    }
                }
            }
        }
        if (aZombieBehind != nullptr)
        {
            return aZombieBehind; // Found a zombie behind, fire normally.
        }

        Zombie* aZombieInFront = FindTargetZombie(theRow, PlantWeapon::WEAPON_PRIMARY);
        if (aZombieInFront != nullptr)
        {
            // Return the front zombie as a "trigger" to make the back head fire.
            return aZombieInFront;
        }
    }

    int aDamageRangeFlags = GetDamageRangeFlags(thePlantWeapon);
    Rect aAttackRect = GetPlantAttackRect(thePlantWeapon);
    int aHighestWeight = 0;
    Zombie* aBestZombie = nullptr;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if ((mSeedType == SEED_CABBAGEPULT || mSeedType == SEED_KERNELPULT || mSeedType == SEED_MELONPULT || mSeedType == SEED_WINTERMELON || mSeedType == SEED_POISONPULT) &&
            (aZombie->mZombiePhase == ZombiePhase::PHASE_ZOMBIE_DYING))
        {
            continue;
        }

        int aRowDeviation = aZombie->mRow - theRow;
        if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
        {
            aRowDeviation = 0;
        }

        if (!aZombie->mHasHead || aZombie->IsTangleKelpTarget())
        {
            if (mSeedType == SeedType::SEED_POTATOMINE || mSeedType == SeedType::SEED_CHOMPER || mSeedType == SeedType::SEED_TANGLEKELP || mSeedType == SeedType::SEED_SUPER_CHOMPER)
            {
                continue;
            }
        }

        bool needPortalCheck = false;
        if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT)
        {
            if (mSeedType == SeedType::SEED_PEASHOOTER || mSeedType == SeedType::SEED_CACTUS || mSeedType == SeedType::SEED_REPEATER)
            {
                needPortalCheck = true;
            }
        }

        if (mSeedType != SeedType::SEED_CATTAIL && mSeedType != SeedType::SEED_COMMANDOPEA && mSeedType != SeedType::SEED_GENERALPEA)
        {
            if (mSeedType == SeedType::SEED_GLOOMSHROOM)
            {
                if (aRowDeviation < -1 || aRowDeviation > 1)
                {
                    continue;
                }
            }
            else if (needPortalCheck)
            {
                if (!mBoard->mChallenge->CanTargetZombieWithPortals(this, aZombie))
                {
                    continue;
                }
            }
            else if (aRowDeviation)
            {
                continue;
            }
        }

        if (aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            int aExtraRange = 0;

            if (mSeedType == SeedType::SEED_CHOMPER || mSeedType == SeedType::SEED_SUPER_CHOMPER)
            {
                if (aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_WALKING)
                {
                    aAttackRect.mX += 20;
                    aAttackRect.mWidth -= 20;
                }

                if (aZombie->mZombiePhase == ZombiePhase::PHASE_POGO_BOUNCING || (aZombie->mZombieType == ZombieType::ZOMBIE_BUNGEE && aZombie->mTargetCol == mPlantCol))
                {
                    continue;
                }

                if (aZombie->mIsEating || mState == PlantState::STATE_CHOMPER_BITING)
                {
                    aExtraRange = 60;
                }
            }

            if (mSeedType == SeedType::SEED_POTATOMINE)
            {
                if ((aZombie->mZombieType == ZombieType::ZOMBIE_POGO && aZombie->mHasObject) ||
                    aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT)
                {
                    continue;
                }

                if (aZombie->mZombieType == ZombieType::ZOMBIE_POLEVAULTER)
                {
                    aAttackRect.mX += 40;
                    aAttackRect.mWidth -= 40;  
                }

                if (aZombie->mZombieType == ZombieType::ZOMBIE_BUNGEE && aZombie->mTargetCol != mPlantCol)
                {
                    continue;
                }

                if (aZombie->mIsEating)
                {
                    aExtraRange = 30;
                }
            }

            if ((mSeedType == SeedType::SEED_EXPLODE_O_NUT && aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT) ||
                (mSeedType == SeedType::SEED_TANGLEKELP && !aZombie->mInPool))
            {
                continue;
            }

            Rect aZombieRect = aZombie->GetZombieRect();
            if (!needPortalCheck && GetRectOverlap(aAttackRect, aZombieRect) < -aExtraRange)
            {
                continue;
            }

            ////////////////////

            int aWeight = -aZombieRect.mX;
            if (mSeedType == SeedType::SEED_CATTAIL || mSeedType == SeedType::SEED_COMMANDOPEA || mSeedType == SeedType::SEED_GENERALPEA)
            {
                float dist = Distance2D(mX + 40.0f, mY + 40.0f, aZombieRect.mX + aZombieRect.mWidth / 2.0f, aZombieRect.mY + aZombieRect.mHeight / 2.0f);
                aWeight = -(int)dist;

                if (mSeedType == SeedType::SEED_CATTAIL && aZombie->IsFlying())
                {
                    aWeight += 10000;  
                }
                else if (mSeedType == SeedType::SEED_COMMANDOPEA || mSeedType == SeedType::SEED_GENERALPEA)
                {
                    if (aZombie->mRow == mRow)
                    {
                        aWeight += 10000;
                    }
                }
            }

            if (aBestZombie == nullptr || aWeight > aHighestWeight)
            {
                aHighestWeight = aWeight;
                aBestZombie = aZombie;
            }
        }
    }

    return aBestZombie;
}

Zombie* Plant::FindSuperChomperTarget()
{
    Rect aAttackRect = GetPlantAttackRect(WEAPON_PRIMARY);
    Zombie* aBestZombie = nullptr;
    int aMinX = 0;
    int aDamageRangeFlags = GetDamageRangeFlags(WEAPON_PRIMARY);

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (aZombie->mRow != mRow)
            continue;

        if (!aZombie->mHasHead || aZombie->IsTangleKelpTarget() || !aZombie->EffectedByDamage((unsigned int)aDamageRangeFlags))
            continue;

        if (aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || aZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_IN_JUMP)
            continue;

        Rect aZombieRect = aZombie->GetZombieRect();
        if (GetRectOverlap(aAttackRect, aZombieRect) > 0)
        {
            if (aBestZombie == nullptr || aZombie->mX < aMinX)
            {
                aBestZombie = aZombie;
                aMinX = aZombie->mX;
            }
        }
    }
    return aBestZombie;
}

Zombie* Plant::FindClosestZombieInRow(int theRow)
{
    Zombie* aClosestZombie = NULL;
    int aClosestDist = 10000;

    Zombie* aZombie = NULL;
    while (mBoard->IterateZombies(aZombie))
    {
        if (aZombie->mRow == theRow && !aZombie->IsDeadOrDying())
        {
            int aDist = abs(aZombie->mX - mX);
            if (aDist < aClosestDist)
            {
                aClosestDist = aDist;
                aClosestZombie = aZombie;
            }
        }
    }
    return aClosestZombie;
}

int Plant::DistanceToClosestZombie()
{
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);
    int aClosestDistance = 1000;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (aZombie->mRow == mRow && aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            int aDistance = -GetRectOverlap(aAttackRect, aZombieRect);
            if (aDistance < aClosestDistance)
            {
                aClosestDistance = max(aDistance, 0);
            }
        }
    }

    return aClosestDistance;
}

void Plant::Die()
{
    if (!mBoard)
    {
        mDead = true;
        RemoveEffects();
        return;
    }

    if (mSeedType == SEED_SUNBEAN_BOMB && !mIsBlowing)
    {
        mIsBlowing = true; // Prevents the explosion from happening more than once

        // Find the zombie that is currently eating this plant
        Zombie* aZombieAttacker = nullptr;
        Rect aPlantRect = GetPlantRect();
        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if (!aZombie->IsDeadOrDying() && aZombie->mIsEating && aZombie->mRow == mRow)
            {
                Rect aZombieRect = aZombie->GetZombieRect();
                if (GetRectOverlap(aPlantRect, aZombieRect) > 0)
                {
                    aZombieAttacker = aZombie;
                    break;
                }
            }
        }

        // Only trigger the full effect if the plant was actually eaten
        if (aZombieAttacker != nullptr)
        {
            // Play explosion effects
            mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB); // Re-use the cherry bomb explosion sound
            mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 1, ParticleEffect::PARTICLE_POWIE);

            // 1. Deal massive damage to the zombie that ate it
            aZombieAttacker->TakeDamage(3200, (1U << DAMAGE_BYPASSES_SHIELD));

            // 2. Damage and make zombies in a 3x3 area drop a large sun
            int aCenterCol = mPlantCol;
            int aCenterRow = mRow;
            Zombie* aZombieInRange = nullptr;
            while (mBoard->IterateZombies(aZombieInRange))
            {
                if (aZombieInRange->IsDeadOrDying()) continue;

                // Check if the zombie is within the 3x3 grid area
                int aZombieCol = mBoard->PixelToGridXKeepOnBoard(aZombieInRange->mX + 40, aZombieInRange->mY + 60);
                if (abs(aZombieInRange->mRow - aCenterRow) <= 1 && abs(aZombieCol - aCenterCol) <= 1)
                {
                    // Don't re-damage the main attacker
                    if (aZombieInRange != aZombieAttacker)
                    {
                        aZombieInRange->TakeDamage(900, (1U << DAMAGE_BYPASSES_SHIELD));
                    }

                    // Make the zombie drop a large sun
                    mBoard->AddCoin(aZombieInRange->mX + 40, aZombieInRange->mY, CoinType::COIN_LARGESUN, CoinMotion::COIN_MOTION_FROM_PLANT);
                }
            }
        }
    }

    if (mSeedType == SeedType::SEED_BLACK_THREEPEATER && mBoard)
    {
        mBoard->AddFirePlace(mPlantCol, mRow, 7); // 7.2 damage/tick = exactly 3,600 total damage over 5 seconds!
    }

    if (mSeedType == SEED_SUNBEAN && mBoard)
    {
        Zombie* aZombieAttacker = nullptr;
        Rect aPlantRect = GetPlantRect();
        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if (!aZombie->IsDeadOrDying() && aZombie->mIsEating && aZombie->mRow == mRow)
            {
                Rect aZombieRect = aZombie->GetZombieRect();
                if (GetRectOverlap(aPlantRect, aZombieRect) > 0)
                {
                    aZombieAttacker = aZombie;
                    break;
                }
            }
        }

        // Only trigger the ability if the plant was actually eaten
        if (aZombieAttacker != nullptr)
        {
            // --- Start of Fix ---

            // 1. First, create a list of all zombies in the 3x3 area BEFORE dealing damage.
            std::vector<Zombie*> aZombiesInArea;
            int aCenterCol = mPlantCol;
            int aCenterRow = mRow;
            Zombie* aZombieInRange = nullptr;
            while (mBoard->IterateZombies(aZombieInRange))
            {
                // We check if they are alive now, but we don't care if they die in the next step.
                if (aZombieInRange->IsDeadOrDying()) continue;

                int aZombieCol = mBoard->PixelToGridX(aZombieInRange->mX, aZombieInRange->mY);
                if (abs(aZombieInRange->mRow - aCenterRow) <= 1 && abs(aZombieCol - aCenterCol) <= 1)
                {
                    aZombiesInArea.push_back(aZombieInRange);
                }
            }

            // 2. Now, deal the damage to the attacker.
            mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 1, ParticleEffect::PARTICLE_JACKEXPLODE);
            mApp->PlayFoley(FOLEY_SPLAT);
            aZombieAttacker->TakeDamage(900, (1U << DAMAGE_BYPASSES_SHIELD));

            // 3. Finally, iterate through our saved list and make each zombie drop a sun.
            for (Zombie* pZombieToDropSun : aZombiesInArea)
            {
                mBoard->AddCoin(pZombieToDropSun->mX + 40, pZombieToDropSun->mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            }
            // --- End of Fix ---
        }
    }

    if (mSeedType == SEED_DOOM_NUT)
    {
        if (mState < STATE_DOOM_NUT_PRE_LAUNCH || mState > STATE_DOOM_NUT_DONE_FALLING)
        {
            Zombie* aTargetZombie = FindClosestZombieInRow(mRow);
            if (aTargetZombie)
            {
                mState = STATE_DOOM_NUT_PRE_LAUNCH;
                mStateCountdown = 50; // Delay before jumping
                mTargetX = aTargetZombie->mX;
                mTargetY = aTargetZombie->mY;
                mStartRow = mRow;
                mApp->PlayFoley(FOLEY_SQUASH_HMM);
                mPlantHealth = mPlantMaxHealth; // Heal it for the duration of the jump
                return; // This is crucial: we exit here to perform the jump instead of dying.
            }
        }
    }

    if (mSeedType == SeedType::SEED_SCAREDYSHROOM && mPlantCol >= 6 && mBoard)
    {
        mBoard->DisplayAdvice(_S("WHY? JUST WHY?"), MessageStyle::MESSAGE_STYLE_HINT_FAST, AdviceType::ADVICE_NONE);
    }

    if ((mSeedType == SEED_EXPLODE_O_NUT || mImitaterType == SEED_EXPLODE_O_NUT) && !mWasShoveled)
    {
        int aPosX = mX + mWidth / 2;
        int aPosY = mY + mHeight / 2;
        mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB);
        mApp->PlayFoley(FoleyType::FOLEY_JUICY);
        mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 115, 1, true, GetDamageRangeFlags(WEAPON_PRIMARY));
        mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POWIE);
        mBoard->ShakeBoard(3, -4);
    }

    if ((mSeedType == SeedType::SEED_TORCHWOOD || mSeedType == SeedType::SEED_BLUE_TORCHWOOD) && IsInPlay())
    {
        mApp->PlayFoley(FoleyType::FOLEY_JALAPENO_IGNITE);
        mBoard->DoFwoosh(mRow);
        BurnRow(mRow);
    }

    if (IsOnBoard() && mSeedType == SeedType::SEED_TANGLEKELP)
    {
        Zombie* aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
        if (aZombie)
        {
            aZombie->DieWithLoot();
        }
    }

    mDead = true;
    RemoveEffects();

    if (!Plant::IsFlying(mSeedType) && IsOnBoard())
    {
        GridItem* aLadder = mBoard->GetLadderAt(mPlantCol, mRow);
        if (aLadder)
        {
            aLadder->GridItemDie();
        }
    }

    if (IsOnBoard())
    {
        Plant* aTopPlant = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_BUNGEE_ORDER);
        Plant* aFlowerPot = mBoard->GetFlowerPotAt(mPlantCol, mRow);
        if (aFlowerPot && aTopPlant == aFlowerPot)
        {
            Reanimation* aPotReanim = mApp->ReanimationGet(aFlowerPot->mBodyReanimID);
            aPotReanim->mAnimRate = RandRangeFloat(10.0f, 15.0f);
        }
    }
}

PlantWeaponDef GetPlantWeaponDef(SeedType theSeedType)
{
    switch (theSeedType)
    {
    case SeedType::SEED_PEASHOOTER:
        return { 1, 25, 26, 45.0f, PATTERN_STRAIGHT, PROJECTILE_PEA };
    case SeedType::SEED_SNOWPEA:
        return { 1, 25, 26, 45.0f, PATTERN_STRAIGHT, PROJECTILE_SNOWPEA };
    case SeedType::SEED_CACTUS:
    case SeedType::SEED_MAD_CACTUS:
        return { 2, 25, 26, 45.0f, PATTERN_STRAIGHT, PROJECTILE_SPIKE };
    case SeedType::SEED_REPEATER:
        return { 2, 25, 26, 45.0f, PATTERN_STRAIGHT, PROJECTILE_PEA };
    case SeedType::SEED_GATLINGPEA:
        return { 4, 20, 20, 45.0f, PATTERN_STRAIGHT, PROJECTILE_PEA };
    case SeedType::SEED_THREEPEATER:
    case SeedType::SEED_BLACK_THREEPEATER:
        return { 1, 25, 26, 45.0f, PATTERN_THREE_ROW, PROJECTILE_PEA };
    case SeedType::SEED_PUFFSHROOM:
    case SeedType::SEED_SCAREDYSHROOM:
    case SeedType::SEED_SEASHROOM:
        return { 1, 25, 25, 35.0f, PATTERN_STRAIGHT, PROJECTILE_PUFF };
    case SeedType::SEED_STARFRUIT:
        return { 1, 25, 25, 35.0f, PATTERN_STAR_5WAY, PROJECTILE_STAR };
    case SeedType::SEED_SOUR_STARFRUIT:
        return { 1, 25, 25, 35.0f, PATTERN_STAR_5WAY, PROJECTILE_RED_STAR };
    case SeedType::SEED_SPLITPEA:
    case SeedType::SEED_SUPER_SPLITPEA:
        return { 2, 25, 26, 45.0f, PATTERN_SPLIT_BACK, PROJECTILE_PEA };
    case SeedType::SEED_COMMANDOPEA:
        return { 1, 25, 26, 45.0f, PATTERN_TARGETED_STRAIGHT, PROJECTILE_PEA_SNIPE };
    case SeedType::SEED_GENERALPEA:
        return { 2, 20, 26, 45.0f, PATTERN_TARGETED_STRAIGHT, PROJECTILE_PEA_SNIPE };
    case SeedType::SEED_CATTAIL:
        return { 2, 25, 25, 35.0f, PATTERN_HOMING, PROJECTILE_SPIKE };
    default:
        return { 1, 25, 26, 45.0f, PATTERN_STRAIGHT, PROJECTILE_PEA };
    }
}

PlantDefinition& GetPlantDefinition(SeedType theSeedType)
{
    if (theSeedType < 0 || theSeedType >= (int)SeedType::NUM_SEED_TYPES ||
        gPlantDefs[theSeedType].mSeedType != theSeedType)
    {
        // Return the definition for SEED_SPROUT as a fallback
        return gPlantDefs[(int)SeedType::SEED_SPROUT];
    }
    return gPlantDefs[theSeedType];
}

int Plant::GetCost(SeedType theSeedType, SeedType theImitaterType)
{
    if (gLawnApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || gLawnApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST)
    {
        if (theSeedType == SeedType::SEED_REPEATER)
        {
            return 600;
        }
        else if (theSeedType == SeedType::SEED_FUMESHROOM)
        {
            return 300;
        }
        else if (theSeedType == SeedType::SEED_TALLNUT)
        {
            return 150;
        }
        else if (theSeedType == SeedType::SEED_BEGHOULED_BUTTON_SHUFFLE)
        {
            return 50;
        }
        else if (theSeedType == SeedType::SEED_BEGHOULED_BUTTON_CRATER)
        {
            return 50;
        }
    }

    switch (theSeedType)
    {
    case SeedType::SEED_SLOT_MACHINE_SUN:           return 0;
    case SeedType::SEED_SLOT_MACHINE_DIAMOND:       return 0;
    case SeedType::SEED_ZOMBIQUARIUM_SNORKLE:       return 100;
    case SeedType::SEED_ZOMBIQUARIUM_TROPHY:        return 1000;
    case SeedType::SEED_ZOMBIE_NORMAL:              return 50;
    case SeedType::SEED_ZOMBIE_TRAFFIC_CONE:        return 75;
    case SeedType::SEED_ZOMBIE_POLEVAULTER:         return 75;
    case SeedType::SEED_ZOMBIE_PAIL:                return 125;
    case SeedType::SEED_ZOMBIE_LADDER:              return 150;
    case SeedType::SEED_ZOMBIE_DIGGER:              return 125;
    case SeedType::SEED_ZOMBIE_BUNGEE:              return 125;
    case SeedType::SEED_ZOMBIE_FOOTBALL:            return 175;
    case SeedType::SEED_ZOMBIE_BALLOON:             return 150;
    case SeedType::SEED_ZOMBIE_SCREEN_DOOR:         return 100;
    case SeedType::SEED_ZOMBONI:                    return 175;
    case SeedType::SEED_ZOMBIE_POGO:                return 200;
    case SeedType::SEED_ZOMBIE_DANCER:              return 350;
    case SeedType::SEED_ZOMBIE_GARGANTUAR:          return 300;
    case SeedType::SEED_ZOMBIE_IMP:                 return 50;
    default:
    {
        if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE)
        {
            const PlantDefinition& aPlantDef = GetPlantDefinition(theImitaterType);
            return aPlantDef.mSeedCost;
        }
        else
        {
            const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
            return aPlantDef.mSeedCost;
        }
    }
    }
}

SexyString Plant::GetNameString(SeedType theSeedType, SeedType theImitaterType)
{
    const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
    SexyString aName = StrFormat(_S("[%s]"), aPlantDef.mPlantName);
    SexyString aTranslatedName = TodStringTranslate(StringToSexyStringFast(aName));

    if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE)
    {
        const PlantDefinition& aImitaterDef = GetPlantDefinition(theImitaterType);
        std::string aImitaterName = StrFormat(_S("[%s]"), aImitaterDef.mPlantName);
        std::string aTranslatedImitaterName = TodStringTranslate(StringToSexyStringFast(aImitaterName));
        return StrFormat(_S("%s %s"), aTranslatedName.c_str(), aTranslatedImitaterName.c_str());
    }

    return aTranslatedName;
}

SexyString Plant::GetToolTip(SeedType theSeedType)
{
    const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
    SexyString aToolTip = StrFormat(_S("[%s_TOOLTIP]"), aPlantDef.mPlantName);
    return TodStringTranslate(aToolTip);
}

int Plant::GetRefreshTime(SeedType theSeedType, SeedType theImitaterType)
{
    if (Challenge::IsZombieSeedType(theSeedType))
    {
        return 0;
    }

    if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE)
    {
        const PlantDefinition& aPlantDef = GetPlantDefinition(theImitaterType);
        return aPlantDef.mRefreshTime;
    }
    else
    {
        const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
        return aPlantDef.mRefreshTime;
    }
}

bool Plant::IsNocturnal(SeedType theSeedtype)
{
    return
        theSeedtype == SeedType::SEED_PUFFSHROOM ||
        theSeedtype == SeedType::SEED_SEASHROOM ||
        theSeedtype == SeedType::SEED_SUNSHROOM ||
        theSeedtype == SeedType::SEED_FUMESHROOM ||
        theSeedtype == SeedType::SEED_HYPNOSHROOM ||
        theSeedtype == SeedType::SEED_DOOMSHROOM ||
        theSeedtype == SeedType::SEED_ICESHROOM ||
        theSeedtype == SeedType::SEED_MAGNETSHROOM ||
        theSeedtype == SeedType::SEED_SCAREDYSHROOM ||
        theSeedtype == SeedType::SEED_GLOOMSHROOM ||
        theSeedtype == SeedType::SEED_NIGHTCAP ||
        theSeedtype == SeedType::SEED_DARKCAP;
}

bool Plant::IsAquatic(SeedType theSeedType)
{
    return
        theSeedType == SeedType::SEED_LILYPAD ||
        theSeedType == SeedType::SEED_TANGLEKELP ||
        theSeedType == SeedType::SEED_SEASHROOM ||
        theSeedType == SeedType::SEED_CATTAIL;
}

bool Plant::IsFlying(SeedType theSeedtype)
{
    return theSeedtype == SeedType::SEED_INSTANT_COFFEE;
}

bool Plant::IsUpgrade(SeedType theSeedtype)
{
    return 
        theSeedtype == SeedType::SEED_GATLINGPEA || 
        theSeedtype == SeedType::SEED_WINTERMELON || 
        theSeedtype == SeedType::SEED_TWINSUNFLOWER || 
        theSeedtype == SeedType::SEED_SPIKEROCK || 
        theSeedtype == SeedType::SEED_COBCANNON || 
        theSeedtype == SeedType::SEED_GOLD_MAGNET || 
        theSeedtype == SeedType::SEED_GLOOMSHROOM || 
        theSeedtype == SeedType::SEED_CATTAIL ||
        theSeedtype == SeedType::SEED_FIRESHOOTER ||
        theSeedtype == SeedType::SEED_ICEPEA ||
        theSeedtype == SeedType::SEED_SUPER_CHOMPER ||
        theSeedtype == SeedType::SEED_EXPLODE_O_NUT ||
        theSeedtype == SeedType::SEED_RED_POTATO_MINE ||
        theSeedtype == SeedType::SEED_HYPNOGIGA ||
        theSeedtype == SeedType::SEED_BOMBSQUASH ||
        theSeedtype == SeedType::SEED_BLACK_THREEPEATER ||
        theSeedtype == SeedType::SEED_BLUE_TORCHWOOD ||
        theSeedtype == SeedType::SEED_DOOM_NUT ||
        theSeedtype == SeedType::SEED_ICE_PLANTERN ||
        theSeedtype == SeedType::SEED_MAD_CACTUS ||
        theSeedtype == SeedType::SEED_SOUR_STARFRUIT ||
        theSeedtype == SeedType::SEED_POISONPULT ||
        theSeedtype == SeedType::SEED_CORROSION_GARLIC ||
        theSeedtype == SeedType::SEED_ABSOLUTELEAF ||
        theSeedtype == SeedType::SEED_SUNBEAN_BOMB ||
        theSeedtype == SeedType::SEED_GENERALPEA ||
        theSeedtype == SeedType::SEED_SWEETEST_POTATO ||
        theSeedtype == SeedType::SEED_HATTREMSAGE ||
        theSeedtype == SeedType::SEED_DARKCAP ||
        theSeedtype == SeedType::SEED_SUPER_SPLITPEA;
}

Rect Plant::GetPlantRect()
{
    Rect aRect;
    if (mSeedType == SeedType::SEED_TALLNUT || mSeedType == SeedType::SEED_DOOM_NUT)
    {
        aRect = Rect(mX + 10, mY, mWidth, mHeight);
    }
    else if (mSeedType == SeedType::SEED_PUMPKINSHELL)
    {
        aRect = Rect(mX, mY, mWidth - 20, mHeight);
    }
    else if (mSeedType == SeedType::SEED_COBCANNON)
    {
        aRect = Rect(mX, mY, 140, 80);
    }
    else
    {
        aRect = Rect(mX + 10, mY, mWidth - 20, mHeight);
    }

    return aRect;
}

Rect Plant::GetPlantAttackRect(PlantWeapon thePlantWeapon)
{
    Rect aRect;
    if (mApp->IsWallnutBowlingLevel())
    {
        aRect = Rect(mX, mY, mWidth - 20, mHeight);
    }
    else if (thePlantWeapon == PlantWeapon::WEAPON_SECONDARY && (mSeedType == SeedType::SEED_SPLITPEA || mSeedType == SeedType::SEED_SUPER_SPLITPEA))
    {
        aRect = Rect(0, mY, mX + 16, mHeight);
    }
    else switch (mSeedType)
    {
    case SeedType::SEED_LEFTPEATER:     aRect = Rect(0,             mY,             mX,                 mHeight);               break;
    case SeedType::SEED_SQUASH:         aRect = Rect(mX + 20,       mY,             mWidth - 35,        mHeight);               break;
    case SeedType::SEED_CHOMPER:        aRect = Rect(mX + 80,       mY,             140,                mHeight);               break;
    case SeedType::SEED_SUPER_CHOMPER:  aRect = Rect(mX + 80,       mY,             240,                mHeight);               break;
    case SeedType::SEED_SPIKEWEED:
    case SeedType::SEED_SPIKEROCK:      aRect = Rect(mX + 20,       mY,             mWidth - 50,        mHeight);               break;
    case SeedType::SEED_POTATOMINE:     aRect = Rect(mX,            mY,             mWidth - 25,        mHeight);               break;
    case SeedType::SEED_RED_POTATO_MINE:     aRect = Rect(mX,       mY,             mWidth - 25,        mHeight);               break;
    case SeedType::SEED_TORCHWOOD:      
    case SeedType::SEED_BLUE_TORCHWOOD: aRect = Rect(mX + 50,       mY,             30,                 mHeight);               break;
    case SeedType::SEED_PUFFSHROOM:
    case SeedType::SEED_SEASHROOM:      aRect = Rect(mX + 60,       mY,             230,                mHeight);               break;
    case SeedType::SEED_FUMESHROOM:     aRect = Rect(mX + 60,       mY,             340,                mHeight);               break;
    case SeedType::SEED_TANGLEKELP:     aRect = Rect(mX,            mY,             mWidth,             mHeight);               break;
    case SeedType::SEED_CATTAIL:
    case SeedType::SEED_COMMANDOPEA:
    case SeedType::SEED_GENERALPEA:     aRect = Rect(-BOARD_WIDTH,  -BOARD_HEIGHT,  BOARD_WIDTH * 2,    BOARD_HEIGHT * 2);      break;
    case SeedType::SEED_SHRINKING_VIOLET: aRect = Rect(mX,          mY,             120,                mHeight);               break;

    case SeedType::SEED_GLOOMSHROOM:
        if (mGloomMode == GLOOM_RANGER || mGloomMode == GLOOM_RAPID_RANGER)
            aRect = Rect(mX - 160, mY - 160, 400, 400); // 5x5 Tile Range
        else
            aRect = Rect(mX - 80, mY - 80, 240, 240); // 3x3 Tile Range
        break;

    default:                            aRect = Rect(mX + 60,       mY,             max(770 - (mX + 60), 0), mHeight);          break;
    }

    return aRect;
}

void Plant::PreloadPlantResources(SeedType theSeedType)
{
    const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
    if (aPlantDef.mReanimationType != ReanimationType::REANIM_NONE)
    {
        ReanimatorEnsureDefinitionLoaded(aPlantDef.mReanimationType, true);
    }

    if (theSeedType == SeedType::SEED_CHERRYBOMB)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_ZOMBIE_CHARRED, true);
    }
    else if (theSeedType == SeedType::SEED_JALAPENO)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_JALAPENO_FIRE, true);
    }
    else if (theSeedType == SeedType::SEED_TORCHWOOD || theSeedType == SeedType::SEED_BLUE_TORCHWOOD)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_FIRE_PEA, true);
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_JALAPENO_FIRE, true);
    }
    else if (Plant::IsNocturnal(theSeedType))
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_SLEEPING, true);
    }
}

void Plant::PlayIdleAnim(float theRate)
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim)
    {
        PlayBodyReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 20, theRate);
        if (mApp->IsIZombieLevel())
        {
            aBodyReanim->mAnimRate = 0.0f;
        }
    }
}

void Plant::FireBlackThreepeater()
{
    mApp->PlayFoley(FoleyType::FOLEY_JUICY);

    TodWeightedArray aProjectileArray[] = {
        { (int)ProjectileType::PROJECTILE_PEA,           30 },
        { (int)ProjectileType::PROJECTILE_SNOWPEA,       25 },
        { (int)ProjectileType::PROJECTILE_ICEPEA,        20 },
        { (int)ProjectileType::PROJECTILE_FIREBALL,      20 },
        { (int)ProjectileType::PROJECTILE_RED_FIRE_PEA,  15 },
        { (int)ProjectileType::PROJECTILE_BLUE_FIRE_PEA, 10 }
    };
    int aProjectileCount = sizeof(aProjectileArray) / sizeof(aProjectileArray[0]);

    float aOriginX = mX + 45;
    float aOriginY = mY + 10;
    float aProjectileSpeed = 3.33f;

    // 12 projectiles per burst
    for (int i = 0; i < 12; i++)
    {
        ProjectileType aProjectileType = (ProjectileType)TodPickFromWeightedArray(aProjectileArray, aProjectileCount);
        float anAngle = DEG_TO_RAD(RandRangeFloat(-35.0f, 35.0f));

        Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder - 1, mRow, aProjectileType);
        if (aProjectile)
        {
            aProjectile->mMotionType = ProjectileMotion::MOTION_SPREAD;
            aProjectile->mVelX = cos(anAngle) * aProjectileSpeed;
            aProjectile->mVelY = sin(anAngle) * aProjectileSpeed;
            aProjectile->mDamageRangeFlags = GetDamageRangeFlags(WEAPON_PRIMARY);
        }
    }
}

void Plant::UpdateDoomnut()
{
    if (mState == STATE_DOOM_NUT_PRE_LAUNCH)
    {
        mStateCountdown--;
        if (mStateCountdown == 0)
        {
            mState = STATE_DOOM_NUT_RISING;
            mStateCountdown = 150; // Time it takes to jump up
        }
    }
    else if (mState == STATE_DOOM_NUT_RISING)
    {
        mStateCountdown--;
        int aTargetCol = mBoard->PixelToGridXKeepOnBoard(mTargetX, mY);

        mX = TodAnimateCurve(50, 0, mStateCountdown, mBoard->GridToPixelX(mPlantCol, mStartRow), mTargetX, TodCurves::CURVE_EASE_IN_OUT);
        mY = TodAnimateCurve(50, 0, mStateCountdown, mBoard->GridToPixelY(mPlantCol, mStartRow), mBoard->GridToPixelY(aTargetCol, mRow) - 120, TodCurves::CURVE_EASE_IN_OUT);

        if (mStateCountdown == 0)
        {
            mState = STATE_DOOM_NUT_FALLING;
            mStateCountdown = 50;
        }
    }
    else if (mState == STATE_DOOM_NUT_FALLING)
    {
        mStateCountdown--;
        int aTargetCol = mBoard->PixelToGridXKeepOnBoard(mTargetX, mY);
        int aDestY = mBoard->GridToPixelY(aTargetCol, mRow) + 8;
        mY = TodAnimateCurve(10, 0, mStateCountdown, aDestY - 120, aDestY, TodCurves::CURVE_LINEAR);

        if (mStateCountdown == 5) // Moment of impact
        {
            mApp->PlayFoley(FoleyType::FOLEY_SQUASH_HMM);
            mApp->PlayFoley(FoleyType::FOLEY_THUMP);
            Zombie* aZombie = nullptr;
            while (mBoard->IterateZombies(aZombie))
            {
                if (aZombie->mRow == mRow)
                {
                    Rect aZombieRect = aZombie->GetZombieRect();
                    if (aZombieRect.mX + aZombieRect.mWidth >= mX && aZombieRect.mX <= mX + 70)
                    {
                        if (aZombie->mZombieType != ZombieType::ZOMBIE_ZAMBONI && !aZombie->IsFlying() && aZombie->mZombiePhase != ZombiePhase::PHASE_RISING_FROM_GRAVE)
                        {
                            aZombie->TakeDamage(1800, 9U);
                        }
                    }
                }
            }
        }

        if (mStateCountdown == 0)
        {
            int aPosX = mX + 40;
            int aPosY = mY + 40;
            mBoard->DamageAllZombiesInRadius(mRow, aPosX, aPosY, 250, 900, 2, true, 127);
            int aGridX = mBoard->PixelToGridXKeepOnBoard(mX, mY);
            bool aHasFlowerPot = (mBoard->GetFlowerPotAt(aGridX, mRow) != nullptr);
            mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_DOOM);
            if (!aHasFlowerPot)
            {
                mBoard->AddACrater(aGridX, mRow)->mGridItemCounter = 18000;
            }
            mBoard->ShakeBoard(15, -15);
            mApp->PlaySample(SOUND_DOOMSHROOM);

            mState = STATE_DOOM_NUT_DONE_FALLING;
            mStateCountdown = 200;
        }
    }
    else if (mState == STATE_DOOM_NUT_DONE_FALLING)
    {
        mStateCountdown--;
        if (mStateCountdown == 0)
        {
            Die(); // This will now properly kill the plant
        }
    }
}

void Plant::UpdateAbsoluteLeaf()
{
    // Cooldown for the push ability
    if (mPushCooldown > 0)
    {
        mPushCooldown--;
    }

    // Proximity push logic for non-Imp zombies
    if (mPushCooldown == 0 && mState != STATE_UMBRELLA_REFLECTING && mState != STATE_UMBRELLA_TRIGGERED)
    {
        Rect aAttackRect(mX, mY, 140, mHeight); // 1 tile range in front
        Zombie* aZombieToPush = nullptr;
        int aMinX = 10000;

        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            // This ability does NOT affect Imps
            if (aZombie->mZombieType == ZOMBIE_IMP)
                continue;

            if (aZombie->mRow == mRow && !aZombie->IsDeadOrDying() && !aZombie->IsFlying() && aZombie->mZombiePhase != ZombiePhase::PHASE_RISING_FROM_GRAVE)
            {
                Rect aZombieRect = aZombie->GetZombieRect();
                if (GetRectOverlap(aAttackRect, aZombieRect) > 0)
                {
                    if (aZombie->mX < aMinX)
                    {
                        aZombieToPush = aZombie;
                        aMinX = aZombie->mX;
                    }
                }
            }
        }

        if (aZombieToPush != nullptr)
        {
            // Calculate HP cost
            int aHpCost = 10 * (1 << (mPushCounter / 5)); // 10 * 2^(pushes/5)

            // Apply damage and push
            TakeDamage(aHpCost, 1U << DAMAGE_BYPASSES_SHIELD);
            aZombieToPush->mPosX += 80.0f; // Push back 1 tiles
            mPushCounter++;
            mPushCooldown = 100; // 1 second cooldown
            mApp->PlayFoley(FOLEY_BONK);
            DoSpecial();
        }
    }

    // Umbrella reflection logic (copied from UpdateUmbrella)
    if (mState == PlantState::STATE_UMBRELLA_TRIGGERED)
    {
        if (mStateCountdown == 0)
        {
            mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PROJECTILE, mRow + 1, 0);
            mState = PlantState::STATE_UMBRELLA_REFLECTING;
        }
    }
    else if (mState == PlantState::STATE_UMBRELLA_REFLECTING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayIdleAnim(0.0f);
            mState = PlantState::STATE_NOTREADY;
            mRenderOrder = CalcRenderOrder();
        }
    }
}

void Plant::UpdateMarigold()
{
    if (mState == STATE_MARIGOLD_JUMPING)
    {
        if (mStateCountdown > 0)
        {
            // Simple jump animation using Y offset
            float aJumpHeight = TodAnimateCurve(50, 0, mStateCountdown, 0, 30, TodCurves::CURVE_BOUNCE);
            mY = mBoard->GridToPixelY(mPlantCol, mRow) - aJumpHeight;
        }
        else
        {
            mJumpCounter++;
            mY = mBoard->GridToPixelY(mPlantCol, mRow); // Return to original position

            // Spawn loot
            mBoard->AddCoin(mX, mY, CoinType::COIN_DIAMOND, CoinMotion::COIN_MOTION_FROM_PLANT);

            for (int i = 0; i < 2; i++)
            {
                TodWeightedArray aSunType[] = {
                    { (int)CoinType::COIN_SUN,           30 },
                    { (int)CoinType::COIN_LARGESUN,      55 },
                    { (int)CoinType::COIN_WHITE_SUN,     10 },
                    { (int)CoinType::COIN_BIG_WHITE_SUN, 5  }
                };
                CoinType aDroppedSun = (CoinType)TodPickFromWeightedArray(aSunType, 4);
                mBoard->AddCoin(mX + RandRangeInt(-20, 20), mY + RandRangeInt(-20, 20), aDroppedSun, CoinMotion::COIN_MOTION_FROM_PLANT);
            }

            if (mJumpCounter >= 3)
            {
                Die();
            }
            else
            {
                mStateCountdown = 50; // Cooldown for the next jump
            }
        }
    }
}

void Plant::GrantPlacementShieldPulse()
{
    if (!mBoard) return;

    bool isIcePlantern = (mSeedType == SeedType::SEED_ICE_PLANTERN || mImitaterType == SeedType::SEED_ICE_PLANTERN);
    int targetShield = isIcePlantern ? 4000 : 2000;

    Plant* aPlantToBuff = nullptr;
    while (mBoard->IteratePlants(aPlantToBuff))
    {
        if (abs(aPlantToBuff->mPlantCol - mPlantCol) <= 1 && abs(aPlantToBuff->mRow - mRow) <= 1)
        {
            if (aPlantToBuff->mSeedType == SeedType::SEED_PLANTERN || 
                aPlantToBuff->mSeedType == SeedType::SEED_ICE_PLANTERN || 
                aPlantToBuff->mImitaterType == SeedType::SEED_PLANTERN || 
                aPlantToBuff->mImitaterType == SeedType::SEED_ICE_PLANTERN || 
                aPlantToBuff->mSeedType == SeedType::SEED_PUMPKINSHELL ||
                aPlantToBuff->mSeedType == SeedType::SEED_FLOWERPOT ||
                aPlantToBuff->mImitaterType == SeedType::SEED_FLOWERPOT)
            {
                continue;
            }

            bool shieldUpdated = false;
            if (targetShield > aPlantToBuff->mShieldHealth)
            {
                aPlantToBuff->mShieldHealth = targetShield;
                shieldUpdated = true;
            }
            if (targetShield > aPlantToBuff->mShieldMaxHealth)
            {
                aPlantToBuff->mShieldMaxHealth = targetShield;
            }

            if (shieldUpdated && aPlantToBuff != this && mApp)
            {
                mApp->AddTodParticle(aPlantToBuff->mX + 40, aPlantToBuff->mY + 40,
                    aPlantToBuff->mRenderOrder + 1, ParticleEffect::PARTICLE_STARBURST)
                    ->OverrideColor(nullptr, isIcePlantern ? Color(143, 143, 255) : Color(143, 255, 143));
            }
        }
    }
}

void Plant::TriggerSuperSplitPeaSpecial()
{
    int aOriginX = mX + 40;
    int aOriginY = mY + 20;

    mApp->PlayFoley(FoleyType::FOLEY_JUICY);
    mApp->PlaySample(SOUND_CHERRYBOMB);
    mBoard->ShakeBoard(6, -6);

    // Multi-ring explosive particle effects!
    mApp->AddTodParticle(aOriginX, aOriginY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POWIE);
    mApp->AddTodParticle(aOriginX, aOriginY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_JACKEXPLODE);
    
    TodParticleSystem* pStarburst = mApp->AddTodParticle(aOriginX, aOriginY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_STARBURST);
    if (pStarburst)
    {
        pStarburst->OverrideColor(nullptr, Color(255, 100, 255));
    }

    const int NUM_PEAS = 60;
    const float PI = 3.14159265358979323846f;
    for (int i = 0; i < NUM_PEAS; i++)
    {
        float aAngle = (float)i * (2.0f * PI / (float)NUM_PEAS);
        Projectile* p = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder + 1, mRow, ProjectileType::PROJECTILE_BOUNCING_PEA);
        if (p)
        {
            float aSpeed = 12.0f;
            p->mVelX = cosf(aAngle) * aSpeed;
            p->mVelY = sinf(aAngle) * aSpeed;
            p->mMotionType = ProjectileMotion::MOTION_STRAIGHT;
            p->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
        }
    }
}

void Plant::CheckAndReceiveNearbyPlanternShield()
{
    if (!mBoard) return;

    int aBestShield = 0;
    bool isIcePlantern = false;

    Plant* aOtherPlant = nullptr;
    while (mBoard->IteratePlants(aOtherPlant))
    {
        if (aOtherPlant == this || aOtherPlant->mDead || aOtherPlant->mSquished) continue;

        if (abs(aOtherPlant->mPlantCol - mPlantCol) <= 1 && abs(aOtherPlant->mRow - mRow) <= 1)
        {
            if (aOtherPlant->mSeedType == SeedType::SEED_ICE_PLANTERN || aOtherPlant->mImitaterType == SeedType::SEED_ICE_PLANTERN)
            {
                if (4000 > aBestShield)
                {
                    aBestShield = 4000;
                    isIcePlantern = true;
                }
            }
            else if (aOtherPlant->mSeedType == SeedType::SEED_PLANTERN || aOtherPlant->mImitaterType == SeedType::SEED_PLANTERN)
            {
                if (2000 > aBestShield)
                {
                    aBestShield = 2000;
                    if (!isIcePlantern) isIcePlantern = false;
                }
            }
        }
    }

    if (aBestShield > 0)
    {
        bool shieldUpdated = false;
        if (aBestShield > mShieldHealth)
        {
            mShieldHealth = aBestShield;
            shieldUpdated = true;
        }
        if (aBestShield > mShieldMaxHealth)
        {
            mShieldMaxHealth = aBestShield;
        }

        if (shieldUpdated && mApp)
        {
            mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 1, ParticleEffect::PARTICLE_STARBURST)
                ->OverrideColor(nullptr, isIcePlantern ? Color(143, 143, 255) : Color(143, 255, 143));
        }
    }
}

void Plant::UpdatePlanternEffects(float healthMultiplier, int healAmount)
{
    if (!mBoard) return;

    mHealCountdown--;
    if (mIsBoosted)
    {
        mHealCountdown -= 2;
    }

    if (mHealCountdown <= 0)
    {
        mHealCountdown = 100;

        Plant* aPlantToBuff = nullptr;
        while (mBoard->IteratePlants(aPlantToBuff))
        {
            if (abs(aPlantToBuff->mPlantCol - mPlantCol) <= 1 && abs(aPlantToBuff->mRow - mRow) <= 1)
            {
                if (aPlantToBuff->mSeedType == SeedType::SEED_PLANTERN || 
                    aPlantToBuff->mSeedType == SeedType::SEED_ICE_PLANTERN || 
                    aPlantToBuff->mImitaterType == SeedType::SEED_PLANTERN || 
                    aPlantToBuff->mImitaterType == SeedType::SEED_ICE_PLANTERN || 
                    aPlantToBuff->mSeedType == SeedType::SEED_PUMPKINSHELL)
                {
                    continue;
                }

                bool isIcePlantern = (mSeedType == SeedType::SEED_ICE_PLANTERN || mImitaterType == SeedType::SEED_ICE_PLANTERN);

                // Heal base plant HP if damaged (Shields are ONE-TIME placement pulses only)
                if (aPlantToBuff->mPlantHealth < aPlantToBuff->mPlantMaxHealth)
                {
                    aPlantToBuff->mPlantHealth += healAmount;
                    if (aPlantToBuff->mPlantHealth > aPlantToBuff->mPlantMaxHealth)
                    {
                        aPlantToBuff->mPlantHealth = aPlantToBuff->mPlantMaxHealth;
                    }

                    if (aPlantToBuff != this && mApp)
                    {
                        mApp->AddTodParticle(aPlantToBuff->mX + 40, aPlantToBuff->mY + 40,
                            aPlantToBuff->mRenderOrder + 1, ParticleEffect::PARTICLE_STARBURST)
                            ->OverrideColor(nullptr, isIcePlantern ? Color(143, 143, 255) : Color(143, 255, 143));
                    }
                }
            }
        }
    }
}

void Plant::TakeDamage(int theDamage, unsigned int theDamageFlags)
{
    if (mShieldHealth > 0)
    {
        if (theDamage <= mShieldHealth)
        {
            mShieldHealth -= theDamage;
        }
        else
        {
            mShieldHealth = 0; // Shield breaks and absorbs the hit
        }
        return; // No damage spillover to base plant HP while shield is active!
    }

    if (theDamage > 0)
    {
        mPlantHealth -= theDamage;
        if (mPlantHealth <= 0)
        {
            mPlantHealth = 0;
            Die();
        }
    }
}

void Plant::GetButterSplatOffset(int& theOffsetX, int& theOffsetY)
{
    // Default offsets that work for medium-sized plants
    theOffsetX = 40;
    theOffsetY = 20;

    // Add specific offsets for each plant type here
    switch (mSeedType)
    {
    case SEED_TALLNUT:
        theOffsetY = -25;
        break;

    case SEED_PUFFSHROOM:
    case SEED_SEASHROOM:
    case SEED_TANGLEKELP:
        theOffsetY = 60;
        break;

    case SEED_LILYPAD:
    case SEED_FLOWERPOT:
        theOffsetY = 80;

    case SEED_CABBAGEPULT:
    case SEED_STARFRUIT:
    case SEED_SUNSHROOM:
    case SEED_CATTAIL:
        theOffsetY = 30;
        break;

    case SEED_CACTUS:
        theOffsetY = 10;
        break;

    case SEED_GARLIC:
    case SEED_PUMPKINSHELL:
        theOffsetY = 40;
        break;


    default:
        break;
    }
}

SeedType Plant::PickRandomSeedType()
{
    return (SeedType)RandRangeInt((int)SEED_PEASHOOTER, (int)SEED_ABSOLUTELEAF);
}

void Plant::UpdateSprout()
{
    // Make the Sprout pulse while it's waiting to transform
    if (mState == STATE_SPROUT_TRANSFORMING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
        if (aBodyReanim)
        {
            float aPulse = TodAnimateCurveFloat(50, 0, mBoard->mMainCounter % 50, 1.0f, 1.1f, CURVE_EASE_SIN_WAVE);
            aBodyReanim->OverrideScale(aPulse, aPulse);
        }

        // When the timer is up, transform it
        if (mStateCountdown <= 0)
        {
            // Choose a random seed
            SeedType aNewSeed = PickRandomSeedType();

            // Create a usable seed packet coin at the Sprout's location
            Coin* aCoin = mBoard->AddCoin(mX, mY, COIN_USABLE_SEED_PACKET, COIN_MOTION_FROM_PLANT);
            if (aCoin)
            {
                aCoin->mUsableSeedType = aNewSeed;
            }

            // Add a nice visual and sound effect
            mApp->PlayFoley(FOLEY_PRIZE);
            mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 1, PARTICLE_PRESENT_PICKUP);

            Die();
        }
    }
}

void Plant::UpdateCommandoPea()
{
    Zombie* aCurrentTarget = mBoard->ZombieTryToGet(mTargetZombieID);
    if (aCurrentTarget == nullptr || aCurrentTarget->IsDeadOrDying())
    {
        aCurrentTarget = FindTargetZombie(mRow, WEAPON_PRIMARY);
        mTargetZombieID = mBoard->ZombieGetID(aCurrentTarget);
    }

    Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mHeadReanimID);
    if (aHeadReanim != nullptr && aCurrentTarget != nullptr && !aCurrentTarget->IsDeadOrDying())
    {
        float aHeadX, aHeadY;
        GetPeaHeadOffset(reinterpret_cast<int&>(aHeadX), reinterpret_cast<int&>(aHeadY));
        aHeadX += mX; aHeadY += mY;

        float aTargetX = aCurrentTarget->mX + 40.0f;
        float aTargetY = aCurrentTarget->mY + 60.0f;
        float anAngle = atan2f(aTargetY - aHeadY, aTargetX - aHeadX);
        aHeadReanim->mOverlayMatrix.LoadIdentity();
        aHeadReanim->mOverlayMatrix.RotateRad(-anAngle);
    }

    // State 1: Ready to find a target
    if (mState == STATE_READY)
    {
        if (mLaunchCounter > 0)
            mLaunchCounter--;

        if (mLaunchCounter == 0)
        {
            if (aCurrentTarget != nullptr && !aCurrentTarget->IsDeadOrDying())
            {
                mState = STATE_COMMANDOPEA_AIMING;
                mStateCountdown = 15; // Aim for 0.15 seconds
            }
        }
    }
    // State 2: Aiming at the target
    else if (mState == STATE_COMMANDOPEA_AIMING)
    {
        mStateCountdown--;

        Zombie* aTarget = mBoard->ZombieTryToGet(mTargetZombieID);
        if (aHeadReanim == nullptr || aTarget == nullptr || aTarget->IsDeadOrDying())
        {
            mState = STATE_READY;
            mLaunchCounter = 30; // Small delay before finding a new target
            if (aHeadReanim) aHeadReanim->mOverlayMatrix.LoadIdentity();
            return;
        }

        // --- Head Rotation Logic ---
        float aHeadX, aHeadY;
        GetPeaHeadOffset(reinterpret_cast<int&>(aHeadX), reinterpret_cast<int&>(aHeadY));
        aHeadX += mX; aHeadY += mY;

        float aTargetX = aTarget->mX + 40;
        float aTargetY = aTarget->mY + 60;
        float anAngle = atan2(aTargetY - aHeadY, aTargetX - aHeadX);
        aHeadReanim->mOverlayMatrix.LoadIdentity();
        aHeadReanim->mOverlayMatrix.RotateRad(-anAngle);
        // --- End Head Rotation ---

        // When aiming is done, fire and transition to the firing state
        if (mStateCountdown <= 0)
        {
            mState = STATE_COMMANDOPEA_FIRING; // Transition to firing state
            mShootingCounter = 33; // Set animation/cooldown timer

            Fire(aTarget, mRow, WEAPON_PRIMARY);

            // Play the head's shooting animation
            if (aHeadReanim)
            {
                aHeadReanim->PlayReanim("anim_shooting", REANIM_PLAY_ONCE_AND_HOLD, 10, 35.0f);
            }
        }
    }
    // State 3: Waiting for the shooting animation to finish
    else if (mState == STATE_COMMANDOPEA_FIRING)
    {
        if (mShootingCounter > 0)
        {
            mShootingCounter--;
        }

        if (mShootingCounter == 0)
        {
            mState = STATE_READY;
            mLaunchCounter = mLaunchRate;

            // Reset head animation back to idle
            Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mHeadReanimID);
            if (aHeadReanim)
            {
                aHeadReanim->mOverlayMatrix.LoadIdentity(); // Reset rotation
                aHeadReanim->PlayReanim("anim_head_idle", REANIM_LOOP, 20, 15.0f);
            }
        }
    }
}

void Plant::DrawTacticalTargetMark(Graphics* g, float theCenterX, float theCenterY, SeedType thePlantType)
{
    if (!mBoard) return;

    int aCounter = mBoard->mMainCounter;
    Color aPrimaryColor = (thePlantType == SeedType::SEED_GENERALPEA) ? Color(0, 240, 255, 255) : Color(255, 40, 40, 255);
    Color aGlowColor = (thePlantType == SeedType::SEED_GENERALPEA) ? Color(0, 180, 255, 180) : Color(255, 30, 30, 180);

    float aRadius = 28.0f + 3.0f * sinf(aCounter * 0.15f);
    float aAngle = aCounter * 0.04f;

    auto DrawThickLine = [&](int x1, int y1, int x2, int y2, int thickness)
    {
        for (int dx = -thickness / 2; dx <= thickness / 2; dx++)
        {
            for (int dy = -thickness / 2; dy <= thickness / 2; dy++)
            {
                g->DrawLine(x1 + dx, y1 + dy, x2 + dx, y2 + dy);
            }
        }
    };

    // --- Pass 1: Solid Opaque Base (High Contrast) ---
    g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
    g->SetColor(aPrimaryColor);

    // 1. Outer Corner Brackets (2px Thick L-shapes)
    for (int i = 0; i < 4; i++)
    {
        float rot = aAngle + i * (3.14159265f / 2.0f);
        float x1 = theCenterX + cosf(rot) * aRadius;
        float y1 = theCenterY + sinf(rot) * aRadius;

        float bracketLen = 10.0f;
        float rotB1 = rot + 2.35619f; // 135 deg
        float rotB2 = rot - 2.35619f;

        DrawThickLine((int)x1, (int)y1, (int)(x1 + cosf(rotB1) * bracketLen), (int)(y1 + sinf(rotB1) * bracketLen), 2);
        DrawThickLine((int)x1, (int)y1, (int)(x1 + cosf(rotB2) * bracketLen), (int)(y1 + sinf(rotB2) * bracketLen), 2);
    }

    // 2. Thick Crosshair Bars (3px Thickness)
    int gap = 8;
    int len = 18;
    int barThick = 3;

    g->FillRect((int)(theCenterX - len), (int)(theCenterY - barThick / 2), len - gap, barThick);
    g->FillRect((int)(theCenterX + gap), (int)(theCenterY - barThick / 2), len - gap, barThick);
    g->FillRect((int)(theCenterX - barThick / 2), (int)(theCenterY - len), barThick, len - gap);
    g->FillRect((int)(theCenterX - barThick / 2), (int)(theCenterY + gap), barThick, len - gap);

    // 3. Center Lock-On Box (10x10 Double Frame + 4x4 Center Dot)
    int boxSize = 10;
    g->DrawRect((int)(theCenterX - boxSize / 2), (int)(theCenterY - boxSize / 2), boxSize, boxSize);
    g->DrawRect((int)(theCenterX - boxSize / 2 - 1), (int)(theCenterY - boxSize / 2 - 1), boxSize + 2, boxSize + 2);
    g->FillRect((int)(theCenterX - 2), (int)(theCenterY - 2), 4, 4);

    // --- Pass 2: Intense Additive Glow Halo ---
    g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
    g->SetColor(aGlowColor);

    g->FillRect((int)(theCenterX - len - 2), (int)(theCenterY - (barThick + 2) / 2), len - gap + 4, barThick + 2);
    g->FillRect((int)(theCenterX + gap - 2), (int)(theCenterY - (barThick + 2) / 2), len - gap + 4, barThick + 2);
    g->FillRect((int)(theCenterX - (barThick + 2) / 2), (int)(theCenterY - len - 2), barThick + 2, len - gap + 4);
    g->FillRect((int)(theCenterX - (barThick + 2) / 2), (int)(theCenterY + gap - 2), barThick + 2, len - gap + 4);

    g->DrawRect((int)(theCenterX - boxSize / 2 - 2), (int)(theCenterY - boxSize / 2 - 2), boxSize + 4, boxSize + 4);

    g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
}

void Plant::UpdateGeneralPea()
{
    Zombie* aCurrentTarget = mBoard->ZombieTryToGet(mTargetZombieID);
    if (aCurrentTarget == nullptr || aCurrentTarget->IsDeadOrDying())
    {
        aCurrentTarget = FindTargetZombie(mRow, WEAPON_PRIMARY);
        mTargetZombieID = mBoard->ZombieGetID(aCurrentTarget);
    }

    Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mHeadReanimID);
    if (aHeadReanim != nullptr && aCurrentTarget != nullptr && !aCurrentTarget->IsDeadOrDying())
    {
        int aHeadX, aHeadY;
        GetPeaHeadOffset(aHeadX, aHeadY);
        float aHeadXFloat = mX + aHeadX;
        float aHeadYFloat = mY + aHeadY;

        float aTargetX = aCurrentTarget->mX + 40.0f;
        float aTargetY = aCurrentTarget->mY + 60.0f;
        float anAngle = atan2f(aTargetY - aHeadYFloat, aTargetX - aHeadXFloat);
        aHeadReanim->mOverlayMatrix.LoadIdentity();
        aHeadReanim->mOverlayMatrix.RotateRad(-anAngle);
    }

    // State 1: Ready to find a target
    if (mState == STATE_READY)
    {
        if (mLaunchCounter > 0)
            mLaunchCounter--;

        if (mLaunchCounter == 0)
        {
            if (aCurrentTarget != nullptr && !aCurrentTarget->IsDeadOrDying())
            {
                mState = STATE_GENERALPEA_AIMING;
                mStateCountdown = 15; // Aim for 0.15 seconds
            }
        }
    }
    // State 2: Aiming at the target
    else if (mState == STATE_GENERALPEA_AIMING)
    {
        mStateCountdown--;

        Zombie* aTarget = mBoard->ZombieTryToGet(mTargetZombieID);
        if (aHeadReanim == nullptr || aTarget == nullptr || aTarget->IsDeadOrDying())
        {
            mState = STATE_READY;
            mLaunchCounter = 30; // Small delay before finding a new target
            if (aHeadReanim) aHeadReanim->mOverlayMatrix.LoadIdentity();
            return;
        }

        // --- Head Rotation Logic ---
        int aHeadX, aHeadY;
        GetPeaHeadOffset(aHeadX, aHeadY);
        float aHeadXFloat = mX + aHeadX;
        float aHeadYFloat = mY + aHeadY;

        float aTargetX = aTarget->mX + 40;
        float aTargetY = aTarget->mY + 60;
        float anAngle = atan2(aTargetY - aHeadYFloat, aTargetX - aHeadXFloat);
        aHeadReanim->mOverlayMatrix.LoadIdentity();
        aHeadReanim->mOverlayMatrix.RotateRad(-anAngle);
        // --- End Head Rotation ---

        // When aiming is done, fire the first shot
        if (mStateCountdown <= 0)
        {
            mState = STATE_GENERALPEA_FIRING;
            mShootingCounter = 33; // Set timer for both shots

            Fire(aTarget, mRow, WEAPON_PRIMARY); // Fire first pea

            if (aHeadReanim)
            {
                aHeadReanim->PlayReanim("anim_shooting", REANIM_PLAY_ONCE_AND_HOLD, 10, 35.0f);
            }
        }
    }
    // State 3: Firing sequence (two shots)
    else if (mState == STATE_GENERALPEA_FIRING)
    {
        if (mShootingCounter > 0)
        {
            mShootingCounter--;
        }

        // Fire the second shot halfway through the firing animation
        if (mShootingCounter == 15)
        {
            Zombie* aTarget = mBoard->ZombieTryToGet(mTargetZombieID);
            if (aTarget != nullptr && !aTarget->IsDeadOrDying())
            {
                Fire(aTarget, mRow, WEAPON_PRIMARY); // Fire second pea
            }
        }

        // When the firing sequence is over, return to ready state
        if (mShootingCounter == 0)
        {
            mState = STATE_READY;
            mLaunchCounter = mLaunchRate;

            Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mHeadReanimID);
            if (aHeadReanim)
            {
                aHeadReanim->mOverlayMatrix.LoadIdentity();
                aHeadReanim->PlayReanim("anim_head_idle", REANIM_LOOP, 20, 15.0f);
            }
        }
    }
}

void Plant::UpdateShrinkingViolet()
{
    // State 1: Arming phase
    if (mState == STATE_NOTREADY)
    {
        if (mStateCountdown > 0) mStateCountdown--;
        else mState = STATE_READY;
        return;
    }

    // State 2: Armed and waiting for a zombie to touch it
    if (mState == STATE_READY)
    {
        Zombie* aTargetZombie = FindTargetZombie(mRow, WEAPON_PRIMARY);
        if (aTargetZombie != nullptr)
        {
            mState = STATE_SHRINKINGVIOLET_TRIGGERED;
            mStateCountdown = 100;

            Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
            if (aBodyReanim)
            {
                for (int i = 0; i < aBodyReanim->mDefinition->mTrackCount; i++)
                {
                    aBodyReanim->mTrackInstances[i].mRenderGroup = RENDER_GROUP_NORMAL;
                }
            }

            PlayBodyReanim("anim_blow", REANIM_PLAY_ONCE_AND_HOLD, 0, 24.0f);
            mApp->PlayFoley(FOLEY_SHOOP);
        }
    }
    // State 3: Applying effect
    else if (mState == STATE_SHRINKINGVIOLET_TRIGGERED)
    {
        mStateCountdown--;
        if (mStateCountdown == 50)
        {
            mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 1, ParticleEffect::PARTICLE_MIND_CONTROL);

            std::vector<Zombie*> aZombiesToShrink;
            Zombie* aZombie = nullptr;
            while (mBoard->IterateZombies(aZombie))
            {
                if (aZombie->IsDeadOrDying()) continue;
                if (abs(aZombie->mRow - mRow) <= 1 && abs(mBoard->PixelToGridX(aZombie->mX, aZombie->mY) - mPlantCol) <= 1)
                {
                    aZombiesToShrink.push_back(aZombie);
                }
            }

            for (Zombie* pZombieToShrink : aZombiesToShrink)
            {
                if (pZombieToShrink->mShrinkLevel >= 2)
                {
                    // Third shrink: Insta-kill
                    mApp->AddTodParticle(pZombieToShrink->mX + 40, pZombieToShrink->mY + 60, mRenderOrder + 1, ParticleEffect::PARTICLE_POW);
                    pZombieToShrink->TakeDamage(1800, 1U << DAMAGE_BYPASSES_SHIELD);
                }
                else
                {
                    // First or second shrink: Recreate the zombie
                    ZombieType aType = pZombieToShrink->mZombieType;
                    int aRow = pZombieToShrink->mRow;
                    float aPosX = pZombieToShrink->mPosX;
                    float aPosY = pZombieToShrink->mPosY;
                    int aFromWave = pZombieToShrink->mFromWave;
                    int aNewBodyHealth = pZombieToShrink->mBodyHealth / 2;
                    int aNewHelmHealth = pZombieToShrink->mHelmHealth / 2;
                    int aNewShieldHealth = pZombieToShrink->mShieldHealth / 2;
                    bool wasMindControlled = pZombieToShrink->mMindControlled;
                    int oldShrinkLevel = pZombieToShrink->mShrinkLevel;

                    pZombieToShrink->DieNoLoot();

                    Zombie* aNewZombie = mBoard->AddZombieInRow(aType, aRow, aFromWave);
                    if (aNewZombie)
                    {
                        aNewZombie->mPosX = aPosX;
                        aNewZombie->mPosY = aPosY;
                        aNewZombie->mShrinkLevel = oldShrinkLevel + 1;

                        // Set scale based on new shrink level
                        if (aNewZombie->mShrinkLevel == 1) aNewZombie->mScaleZombie = 0.5f;
                        else if (aNewZombie->mShrinkLevel >= 2) aNewZombie->mScaleZombie = 0.25f;

                        aNewZombie->mBodyHealth = aNewBodyHealth;
                        aNewZombie->mBodyMaxHealth /= (2 * aNewZombie->mShrinkLevel);
                        aNewZombie->mHelmHealth = aNewHelmHealth;
                        aNewZombie->mHelmMaxHealth /= (2 * aNewZombie->mShrinkLevel);
                        aNewZombie->mShieldHealth = aNewShieldHealth;
                        aNewZombie->mShieldMaxHealth /= (2 * aNewZombie->mShrinkLevel);

                        if (aNewZombie->mBodyHealth < 1) aNewZombie->mBodyHealth = 1;
                        if (wasMindControlled) aNewZombie->StartMindControlled();
                    }
                }
            }
        }

        if (mStateCountdown <= 0)
        {
            Die();
        }
    }
}

Zombie* Plant::FindClosestValidHattremTarget()
{
    Zombie* aBestZombie = nullptr;
    float aBestDistanceSq = -1.0f;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (aZombie->IsDeadOrDying() || !aZombie->IsOnBoard() || aZombie->mMindControlled)
            continue;

        float aDeltaX = mX - aZombie->mX;
        float aDeltaY = mY - aZombie->mY;
        float aDistSq = (aDeltaX * aDeltaX) + (aDeltaY * aDeltaY);
        if (aBestZombie == nullptr || aDistSq < aBestDistanceSq)
        {
            aBestZombie = aZombie;
            aBestDistanceSq = aDistSq;
        }
    }
    return aBestZombie;
}

void Plant::UpdateHattremWitch()
{
    if (mIsAsleep)
        return;

    // State 1: Ready to find a target
    if (mState == STATE_READY)
    {
        if (mLaunchCounter > 0)
            mLaunchCounter--;

        if (mLaunchCounter == 0)
        {
            Zombie* aTarget = FindClosestValidHattremTarget();
            if (aTarget != nullptr)
            {
                // A target is found, begin the attack sequence
                mState = STATE_HATTREM_SHOOTING;
                mShootingCounter = 100; // Total duration for the attack sequence
                mTargetZombieID = mBoard->ZombieGetID(aTarget);

                // Tell BOTH the body and head to play their shooting animations
                Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
                Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mHeadReanimID);

                if (aBodyReanim)
                    aBodyReanim->PlayReanim("anim_shooting", REANIM_PLAY_ONCE_AND_HOLD, 10, 24.0f);
                if (aHeadReanim)
                    aHeadReanim->PlayReanim("anim_shooting", REANIM_PLAY_ONCE_AND_HOLD, 10, 24.0f);
            }
        }
    }
    else if (mState == STATE_HATTREM_SHOOTING)
    {
        if (mShootingCounter > 0)
            mShootingCounter--;

        if (mShootingCounter == 50)
        {
            Zombie* aTarget = mBoard->ZombieTryToGet(mTargetZombieID);
            if (aTarget != nullptr && !aTarget->IsDeadOrDying())
            {
                int aZombieHP = aTarget->mBodyHealth + aTarget->mHelmHealth + aTarget->mShieldHealth + aTarget->mFlyingHealth;
                if (aZombieHP > 300)
                {
                    // Deal 300 damage if HP > 300
                    mApp->PlayFoley(FOLEY_SQUASH_HMM);
                    aTarget->TakeDamage(300, (1U << DAMAGE_BYPASSES_SHIELD));
                }
                else
                {
                    // Otherwise, transform zombie (or deal kill damage if tile is occupied)
                    int aGridX = mBoard->PixelToGridX(aTarget->mX, aTarget->mY);
                    int aGridY = aTarget->mRow;
                    Plant* aPlantOnTile = mBoard->GetTopPlantAt(aGridX, aGridY, TOPPLANT_ANY);

                    if (aPlantOnTile != nullptr)
                    {
                        // Tile is occupied, deal massive damage
                        mApp->PlayFoley(FOLEY_SQUASH_HMM);
                        aTarget->TakeDamage(3000, (1U << DAMAGE_BYPASSES_SHIELD));
                    }
                    else
                    {
                        // Tile is empty, transform into Fume-shroom
                        mApp->PlayFoley(FOLEY_FLOOP);
                        mApp->AddTodParticle(aTarget->mX + 60, aTarget->mY + 60, aTarget->mRenderOrder + 1, PARTICLE_MIND_CONTROL);
                        aTarget->DieNoLoot();
                        if (mBoard->StageHasRoof())
                        {
                            mBoard->AddPlant(aGridX, aGridY, SEED_FLOWERPOT, SEED_NONE);
                        }
                        Plant* aNewPlant = mBoard->AddPlant(aGridX, aGridY, SEED_FUMESHROOM, SEED_NONE);
                        if (aNewPlant && aNewPlant->mIsAsleep)
                        {
                            aNewPlant->SetSleeping(false);
                        }
                    }
                }
            }
        }

        // When the animation is finished, return to the ready state
        if (mShootingCounter == 0)
        {
            mState = STATE_READY;
            mLaunchCounter = mLaunchRate;
            mTargetZombieID = ZOMBIEID_NULL;

            // Tell BOTH the body and head to go back to their idle animations
            Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
            Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mHeadReanimID);

            if (aBodyReanim)
                aBodyReanim->PlayReanim("anim_idle", REANIM_LOOP, 20, aBodyReanim->mAnimRate);
            if (aHeadReanim)
                aHeadReanim->PlayReanim("anim_head_idle", REANIM_LOOP, 20, aHeadReanim->mAnimRate);
        }
    }
}

void Plant::UpdateHattremSage()
{
    if (mIsAsleep)
        return;

    // State 1: Ready to find a target
    if (mState == STATE_READY)
    {
        if (mLaunchCounter > 0)
            mLaunchCounter--;

        if (mLaunchCounter == 0)
        {
            Zombie* aTarget = FindClosestValidHattremTarget();
            if (aTarget != nullptr)
            {
                // A target is found, begin the attack sequence
                mState = STATE_HATTREM_SHOOTING;
                mShootingCounter = 100; // Total duration for the attack sequence
                mTargetZombieID = mBoard->ZombieGetID(aTarget);

                // Tell BOTH the body and head to play their shooting animations
                Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
                Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mHeadReanimID);

                if (aBodyReanim)
                    aBodyReanim->PlayReanim("anim_shooting", REANIM_PLAY_ONCE_AND_HOLD, 10, 24.0f);
                if (aHeadReanim)
                    aHeadReanim->PlayReanim("anim_shooting", REANIM_PLAY_ONCE_AND_HOLD, 10, 24.0f);
            }
        }
    }
    else if (mState == STATE_HATTREM_SHOOTING)
    {
        if (mShootingCounter > 0)
            mShootingCounter--;

        if (mShootingCounter == 50)
        {
            Zombie* aTarget = mBoard->ZombieTryToGet(mTargetZombieID);
            if (aTarget != nullptr && !aTarget->IsDeadOrDying())
            {
                int aZombieHP = aTarget->mBodyHealth + aTarget->mHelmHealth + aTarget->mShieldHealth + aTarget->mFlyingHealth;
                if (aZombieHP > 400)
                {
                    // Deal 400 damage if HP > 400
                    mApp->PlayFoley(FOLEY_SQUASH_HMM);
                    aTarget->TakeDamage(400, (1U << DAMAGE_BYPASSES_SHIELD));
                }
                else
                {
                    // Otherwise, transform zombie (or deal kill damage if tile is occupied)
                    int aGridX = mBoard->PixelToGridX(aTarget->mX, aTarget->mY);
                    int aGridY = aTarget->mRow;
                    Plant* aPlantOnTile = mBoard->GetTopPlantAt(aGridX, aGridY, TOPPLANT_ANY);

                    if (aPlantOnTile != nullptr)
                    {
                        mApp->PlayFoley(FOLEY_SQUASH_HMM);
                        aTarget->TakeDamage(3000, (1U << DAMAGE_BYPASSES_SHIELD));
                    }
                    else
                    {
                        mApp->PlayFoley(FOLEY_FLOOP);
                        mApp->AddTodParticle(aTarget->mX + 60, aTarget->mY + 60, aTarget->mRenderOrder + 1, PARTICLE_MIND_CONTROL);
                        aTarget->DieNoLoot();
                        if (mBoard->StageHasRoof())
                        {
                            mBoard->AddPlant(aGridX, aGridY, SEED_FLOWERPOT, SEED_NONE);
                        }

                        SeedType aNewSeedType = SEED_FUMESHROOM;
                        int aRand = Rand(100);

                        if (aRand < 40) { aNewSeedType = SEED_FUMESHROOM; }        // 40%
                        else if (aRand < 60) { aNewSeedType = SEED_SQUASH; }       // 20%
                        else if (aRand < 85) { aNewSeedType = SEED_WALLNUT; }      // 25%
                        else if (aRand < 95) { aNewSeedType = SEED_TALLNUT; }      // 10%
                        else if (aRand < 99) { aNewSeedType = SEED_CHERRYBOMB; }   // 4%
                        else { aNewSeedType = SEED_DOOMSHROOM; }                   // 1%

                        Plant* aNewPlant = mBoard->AddPlant(aGridX, aGridY, aNewSeedType, SEED_NONE);
                        if (aNewPlant && aNewPlant->mIsAsleep)
                        {
                            aNewPlant->SetSleeping(false);
                        }
                    }
                }
            }
        }

        // When the animation is finished, return to the ready state
        if (mShootingCounter == 0)
        {
            mState = STATE_READY;
            mLaunchCounter = mLaunchRate;
            mTargetZombieID = ZOMBIEID_NULL;

            // Tell BOTH the body and head to go back to their idle animations
            Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
            Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mHeadReanimID);

            if (aBodyReanim)
                aBodyReanim->PlayReanim("anim_idle", REANIM_LOOP, 20, aBodyReanim->mAnimRate);
            if (aHeadReanim)
                aHeadReanim->PlayReanim("anim_head_idle", REANIM_LOOP, 20, aHeadReanim->mAnimRate);
        }
    }
}

Zombie* Plant::FindNightcapTarget()
{
    Rect aAttackRect(mX, mY, 240, mHeight); // 3 tiles in front
    Zombie* aBestZombie = nullptr;
    int aMinX = 999999;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (aZombie->mRow == mRow && !aZombie->IsDeadOrDying() && aZombie->EffectedByDamage(1U << DAMAGES_GROUND))
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            if (GetRectOverlap(aAttackRect, aZombieRect) > 0)
            {
                if (aBestZombie == nullptr || aZombie->mX < aMinX)
                {
                    aBestZombie = aZombie;
                    aMinX = aZombie->mX;
                }
            }
        }
    }
    return aBestZombie;
}

Zombie* Plant::FindDarkCapTarget()
{
    Rect aAttackRect(mX, mY, 320, mHeight); // 4 tiles in front
    Zombie* aBestZombie = nullptr;
    int aMinX = 999999;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (aZombie->mRow == mRow && !aZombie->IsDeadOrDying() && aZombie->EffectedByDamage(1U << DAMAGES_GROUND))
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            if (GetRectOverlap(aAttackRect, aZombieRect) > 0)
            {
                if (aBestZombie == nullptr || aZombie->mX < aMinX)
                {
                    aBestZombie = aZombie;
                    aMinX = aZombie->mX;
                }
            }
        }
    }
    return aBestZombie;
}

void Plant::UpdateNightcap()
{
    if (mIsAsleep) return;

    bool aHasPumpkin = (mBoard && mBoard->GetPumpkinAt(mPlantCol, mRow) != nullptr);
    bool aZombieIsClose = false;

    if (!aHasPumpkin)
    {
        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if (!aZombie->mMindControlled && !aZombie->IsDeadOrDying())
            {
                Rect aZombieRect = aZombie->GetZombieRect();
                int aDiffY = (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) ? 0 : (aZombie->mRow - mRow);
                if (aDiffY == 0 && GetCircleRectOverlap(mX + 40.0f, mY + 40.0f, 60, aZombieRect))
                {
                    aZombieIsClose = true;
                    break;
                }
            }
        }
    }

    if (aZombieIsClose)
    {
        if (!mIsHiding)
        {
            mIsHiding = true;
            mState = STATE_READY;
            mShootingCounter = 0;
            PlayBodyReanim("anim_sleep", REANIM_LOOP, 10, 15.0f);
        }
        return;
    }
    else if (mIsHiding)
    {
        mIsHiding = false;
        mState = STATE_READY;
        mShootingCounter = 0;
        PlayBodyReanim("anim_idle", REANIM_LOOP, 10, 15.0f);
    }

    if (mState == STATE_NIGHTCAP_ATTACKING)
    {
        return;
    }

    if (mLaunchCounter > 0)
        mLaunchCounter--;

    if (mLaunchCounter == 0)
    {
        Zombie* aTarget = FindNightcapTarget();
        if (aTarget != nullptr)
        {
            mState = STATE_NIGHTCAP_ATTACKING;
            mShootingCounter = 100;
            PlayBodyReanim("anim_shooting", REANIM_PLAY_ONCE_AND_HOLD, 10, 36.0f);
        }
    }
}

void Plant::UpdateDarkcap()
{
    if (mIsAsleep) return;

    bool aHasPumpkin = (mBoard && mBoard->GetPumpkinAt(mPlantCol, mRow) != nullptr);
    bool aZombieIsClose = false;

    if (!aHasPumpkin)
    {
        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if (!aZombie->mMindControlled && !aZombie->IsDeadOrDying())
            {
                Rect aZombieRect = aZombie->GetZombieRect();
                int aDiffY = (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) ? 0 : (aZombie->mRow - mRow);
                if (aDiffY == 0 && GetCircleRectOverlap(mX + 40.0f, mY + 40.0f, 60, aZombieRect))
                {
                    aZombieIsClose = true;
                    break;
                }
            }
        }
    }

    if (aZombieIsClose)
    {
        if (!mIsHiding)
        {
            mIsHiding = true;
            mState = STATE_READY;
            mShootingCounter = 0;
            PlayBodyReanim("anim_sleep", REANIM_LOOP, 10, 15.0f);
        }
        return;
    }
    else if (mIsHiding)
    {
        mIsHiding = false;
        mState = STATE_READY;
        mShootingCounter = 0;
        PlayBodyReanim("anim_idle", REANIM_LOOP, 10, 15.0f);
    }

    if (mState == STATE_NIGHTCAP_ATTACKING)
    {
        return;
    }

    if (mLaunchCounter > 0)
        mLaunchCounter--;

    if (mLaunchCounter == 0)
    {
        Zombie* aTarget = FindDarkCapTarget();
        if (aTarget != nullptr)
        {
            mState = STATE_NIGHTCAP_ATTACKING;
            mShootingCounter = 100;
            PlayBodyReanim("anim_shooting", REANIM_PLAY_ONCE_AND_HOLD, 10, 36.0f);
        }
    }
}

void Plant::UpdateSweetPotato()
{
    if (mBoard->mMainCounter % 100 != 0)
        return;

    Rect aLureRect(mX, mY - 100, 160, 300);
    int aLanesToAffect[] = { mRow - 1, mRow + 1 };

    for (int aLane : aLanesToAffect)
    {
        if (aLane < 0 || aLane >= MAX_GRID_SIZE_Y)
            continue;

        bool aLaneHasSweetPotato = false;
        Plant* aPlant = nullptr;
        while (mBoard->IteratePlants(aPlant))
        {
            if (aPlant->mRow == aLane && aPlant->mSeedType == SEED_SWEETPOTATO)
            {
                aLaneHasSweetPotato = true;
                break;
            }
        }
        if (aLaneHasSweetPotato)
            continue;

        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if (aZombie->mRow == aLane && !aZombie->IsDeadOrDying())
            {
                Rect aZombieRect = aZombie->GetZombieRect();
                if (GetRectOverlap(aLureRect, aZombieRect) > 0)
                {
                    aZombie->StopEating();
                    aZombie->StartWalkAnim(20);
                    aZombie->SetRow(mRow);
                }
            }
        }
    }
}

void Plant::UpdateSweetestPotato()
{
    if (mBoard->mMainCounter % 100 != 0)
        return;

    Rect aLureRect(mX, mY - 100, 160, 300);
    int aLanesToAffect[] = { mRow - 1, mRow + 1 };

    for (int aLane : aLanesToAffect)
    {
        if (aLane < 0 || aLane >= MAX_GRID_SIZE_Y)
            continue;

        bool aLaneHasSweetPotato = false;
        Plant* aPlant = nullptr;
        while (mBoard->IteratePlants(aPlant))
        {
            if (aPlant->mRow == aLane && (aPlant->mSeedType == SEED_SWEETPOTATO || aPlant->mSeedType == SEED_SWEETEST_POTATO))
            {
                aLaneHasSweetPotato = true;
                break;
            }
        }
        if (aLaneHasSweetPotato)
            continue;

        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if (aZombie->mRow == aLane && !aZombie->IsDeadOrDying())
            {
                Rect aZombieRect = aZombie->GetZombieRect();
                if (GetRectOverlap(aLureRect, aZombieRect) > 0)
                {
                    // Lure the zombie to the current lane
                    aZombie->StopEating();
                    aZombie->StartWalkAnim(20);
                    aZombie->SetRow(mRow);

                    // NEW LOGIC: Apply a random status effect if the zombie doesn't have one
                    if (!aZombie->HasStatusEffect() && Rand(100) < 50) // 50% chance to apply an effect
                    {
                        int effectType = Rand(5); // Randomly choose between 3 standard effects
                        switch (effectType)
                        {
                        case 0: // Freeze
                            mApp->PlayFoley(FOLEY_FROZEN);
                            aZombie->mChilledCounter = 600;
                            break;
                        case 1: // Hypnotize
                            mApp->PlayFoley(FoleyType::FOLEY_FLOOP);
                            aZombie->StartMindControlled();
                            break;
                        case 2: // Chill (Slow)
                            aZombie->ApplyChill(true);
                            break;
                        case 3:
                            aZombie->mIsRadioactive = true;
                            break;
                        case 4:
                            aZombie->mIsOrange = true;
                            aZombie->mOrangeEffectCountdown = 500;
                            break;
                        }
                    }
                }
            }
        }
    }
}