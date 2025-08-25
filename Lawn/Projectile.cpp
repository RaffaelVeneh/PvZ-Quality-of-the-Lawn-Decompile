#include "Board.h"
#include "Plant.h"
#include "Zombie.h"
#include "Cutscene.h"
#include "Projectile.h"
#include "../LawnApp.h"
#include "../Resources.h"
#include "../GameConstants.h"
#include "../Sexy.TodLib/TodFoley.h"
#include "../Sexy.TodLib/TodDebug.h"
#include "../Sexy.TodLib/Reanimator.h"
#include "../Sexy.TodLib/Attachment.h"

ProjectileDefinition gProjectileDefinition[] = {  
	{ ProjectileType::PROJECTILE_PEA,           0,  20  },
	{ ProjectileType::PROJECTILE_SNOWPEA,       0,  20  },
	{ ProjectileType::PROJECTILE_CABBAGE,       0,  40  },
	{ ProjectileType::PROJECTILE_MELON,         0,  80  },
	{ ProjectileType::PROJECTILE_PUFF,          0,  20  },
	{ ProjectileType::PROJECTILE_WINTERMELON,   0,  80  },
	{ ProjectileType::PROJECTILE_FIREBALL,      0,  40  },
	{ ProjectileType::PROJECTILE_STAR,          0,  20  },
	{ ProjectileType::PROJECTILE_SPIKE,         0,  20  },
	{ ProjectileType::PROJECTILE_BASKETBALL,    0,  75  },
	{ ProjectileType::PROJECTILE_KERNEL,        0,  20  },
	{ ProjectileType::PROJECTILE_COBBIG,        0,  300 },
	{ ProjectileType::PROJECTILE_BUTTER,        0,  80  },
	{ ProjectileType::PROJECTILE_ZOMBIE_PEA,    0,  20  },
	{ ProjectileType::PROJECTILE_GRAPESHOT,     0,  800 },
	{ ProjectileType::PROJECTILE_ICEPEA,        0,  10  },
	{ ProjectileType::PROJECTILE_RED_FIRE_PEA,  0,  80  },
	{ ProjectileType::PROJECTILE_BLUE_FIRE_PEA, 0,  120 },
	{ ProjectileType::PROJECTILE_WHITE_FIRE_PEA,0,  320 },
	{ ProjectileType::PROJECTILE_BLUE_SPIKE,	0,  40  },
	{ ProjectileType::PROJECTILE_BLACK_SPIKE,   0,  80  },
	{ ProjectileType::PROJECTILE_BOUNCING_PEA,  0,  30  },
	{ ProjectileType::PROJECTILE_RED_STAR,      0,  40  },
	{ ProjectileType::PROJECTILE_ARMOR,         0,  600 },
	{ ProjectileType::PROJECTILE_POISON_CABBAGE,0,  40  },
	{ ProjectileType::PROJECTILE_BIG_BUTTER,    0,  80  },
	{ ProjectileType::PROJECTILE_BIG_PEA,       0,  100 },
	{ ProjectileType::PROJECTILE_BIG_FIREPEA,   0,  200 },
	{ ProjectileType::PROJECTILE_BLACK_FIRE_PEA,0,  600 },
	{ ProjectileType::PROJECTILE_BIG_POISON_CABBAGE, 0, 80 },
	{ ProjectileType::PROJECTILE_ZOMBIE_SNOWPEA,0,  20  },
	{ ProjectileType::PROJECTILE_ZOMBIE_PUFF,   0,  20  },
	{ ProjectileType::PROJECTILE_ZOMBIE_SPIKE,  0,  15  },
	{ ProjectileType::PROJECTILE_ZOMBIE_STAR,   0,  20  },
	{ ProjectileType::PROJECTILE_ZOMBIE_CABBAGE,0,  40  },
	{ ProjectileType::PROJECTILE_ZOMBIE_KERNEL, 0,  20  }, 
	{ ProjectileType::PROJECTILE_ZOMBIE_BUTTER, 0,  80  },
};

Projectile::Projectile()
{
}

Projectile::~Projectile()
{
	AttachmentDie(mAttachmentID);
}

void Projectile::ProjectileInitialize(int theX, int theY, int theRenderOrder, int theRow, ProjectileType theProjectileType)
{
	int aGridX = mBoard->PixelToGridXKeepOnBoard(theX, theY);
	mProjectileType = theProjectileType;
	mPosX = theX;
	mPosY = theY;
	mPosZ = 0.0f;
	mVelX = 0.0f;
	mVelY = 0.0f;
	mVelZ = 0.0f;
	mAccZ = 0.0f;
	mShadowY = mBoard->GridToPixelY(aGridX, theRow) + 67.0f;
	mHitTorchwoodGridX = -1;
	mMotionType = ProjectileMotion::MOTION_STRAIGHT;
	mFrame = 0;
	mNumFrames = 1;
	mRow = theRow;
	mCobTargetX = 0.0f;
	mDamageRangeFlags = 0;
	mDead = false;
	mAttachmentID = AttachmentID::ATTACHMENTID_NULL;
	mCobTargetRow = 0;
	mHasBounced = false;
	mTargetZombieID = ZombieID::ZOMBIEID_NULL;
	mOnHighGround = mBoard->mGridSquareType[aGridX][theRow] == GridSquareType::GRIDSQUARE_HIGH_GROUND;
	mPierceCounter = 0;
	for (int i = 0; i < 10; i++)
	{
		mHitPlants[i] = PLANTID_NULL;
	}
	mArmorType = MAGNET_ITEM_NONE;
	if (mBoard->StageHasRoof())
	{
		mShadowY -= 12.0f;
	}
	mRenderOrder = theRenderOrder;
	mRotation = 0.0f;
	mRotationSpeed = 0.0f;
	mWidth = 40;
	mHeight = 40;
	mProjectileAge = 0;
	mClickBackoffCounter = 0;
	mAnimTicksPerFrame = 0;

	if (mProjectileType == PROJECTILE_BOUNCING_PEA)
	{
		mVelX = 3.33f;
	}

	if (mProjectileType == ProjectileType::PROJECTILE_CABBAGE || mProjectileType == ProjectileType::PROJECTILE_BUTTER || 
		mProjectileType == ProjectileType::PROJECTILE_BIG_BUTTER || mProjectileType == ProjectileType::PROJECTILE_POISON_CABBAGE || 
		mProjectileType == ProjectileType::PROJECTILE_BIG_POISON_CABBAGE || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_CABBAGE ||
		mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_KERNEL || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_BUTTER)
	{
		mRotation = -7 * PI / 25;  // DEG_TO_RAD(-50.4f);
		mRotationSpeed = RandRangeFloat(-0.08f, -0.02f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_MELON || mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
	{
		mRotation = -2 * PI / 5;  // DEG_TO_RAD(-72.0f);
		mRotationSpeed = RandRangeFloat(-0.08f, -0.02f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_KERNEL)
	{
		mRotation = 0.0f;
		mRotationSpeed = RandRangeFloat(-0.2f, -0.08f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
	{
		TodParticleSystem* aParticle = mApp->AddTodParticle(mPosX + 8.0f, mPosY + 13.0f, 400000, ParticleEffect::PARTICLE_SNOWPEA_TRAIL);
		AttachParticle(mAttachmentID, aParticle, 8.0f, 13.0f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
	{
		Reanimation* aFirePeaReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_FIRE_PEA);
		aFirePeaReanim->SetPosition(-25.0f, -25.0f);
		aFirePeaReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
		aFirePeaReanim->mAnimRate = RandRangeFloat(50.0f, 80.0f);
		AttachReanim(mAttachmentID, aFirePeaReanim, -25.0f, -25.0f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_RED_FIRE_PEA || mProjectileType == ProjectileType::PROJECTILE_BLUE_FIRE_PEA)
	{
		Reanimation* aFirePeaReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_FIRE_PEA);
		aFirePeaReanim->SetPosition(-25.0f, -25.0f);
		aFirePeaReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
		aFirePeaReanim->mAnimRate = RandRangeFloat(50.0f, 80.0f);
		AttachReanim(mAttachmentID, aFirePeaReanim, -25.0f, -25.0f);
	}
	else if (mProjectileType == PROJECTILE_BIG_FIREPEA)
	{
		Reanimation* aFirePeaReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_FIRE_PEA);
		mApp->PlayFoley(FoleyType::FOLEY_IGNITE);
		aFirePeaReanim->SetPosition(-25.0f, -25.0f);
		aFirePeaReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
		aFirePeaReanim->mAnimRate = RandRangeFloat(50.0f, 80.0f);
		aFirePeaReanim->OverrideScale(2.0f, 2.0f);
		AttachReanim(mAttachmentID, aFirePeaReanim, -25.0f, -25.0f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		mWidth = IMAGE_REANIM_COBCANNON_COB->GetWidth();
		mHeight = IMAGE_REANIM_COBCANNON_COB->GetHeight();
		mRotation = PI / 2;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PUFF || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PUFF)
	{
		TodParticleSystem* aParticle = mApp->AddTodParticle(mPosX + 13.0f, mPosY + 13.0f, 400000, ParticleEffect::PARTICLE_PUFFSHROOM_TRAIL);
		AttachParticle(mAttachmentID, aParticle, 13.0f, 13.0f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BASKETBALL)
	{
		mRotation = RandRangeFloat(0.0f, 2 * PI);
		mRotationSpeed = RandRangeFloat(0.05f, 0.1f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_STAR)
	{
		mShadowY += 15.0f;
		mRotationSpeed = RandRangeFloat(0.05f, 0.1f);
		if (Rand(2) == 0)
		{
			mRotationSpeed = -mRotationSpeed;
		}
	}
	mAnimCounter = 0;
	mX = (int)mPosX;
	mY = (int)mPosY;
}

Plant* Projectile::FindCollisionTargetPlant()
{
	Rect aProjectileRect = GetProjectileRect();

	Plant* aPlant = nullptr;
	while (mBoard->IteratePlants(aPlant))
	{
		if (aPlant->mRow != mRow)
			continue;

		if (mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PEA || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_SNOWPEA ||
			mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PUFF || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_SPIKE)
		{
			if (aPlant->mSeedType == SeedType::SEED_PUFFSHROOM ||
				aPlant->mSeedType == SeedType::SEED_SUNSHROOM ||
				aPlant->mSeedType == SeedType::SEED_POTATOMINE ||
				aPlant->mSeedType == SeedType::SEED_RED_POTATO_MINE ||
				aPlant->mSeedType == SeedType::SEED_SPIKEWEED ||
				aPlant->mSeedType == SeedType::SEED_SPIKEROCK ||
				aPlant->mSeedType == SeedType::SEED_LILYPAD ||
				aPlant->mSeedType == SeedType::SEED_GARLIC ||
				aPlant->mSeedType == SeedType::SEED_CORROSION_GARLIC)
				continue;
		}

		if (mProjectileType == PROJECTILE_ZOMBIE_SPIKE)
		{
			bool alreadyHit = false;
			PlantID aPlantID = static_cast<PlantID>(mBoard->mPlants.DataArrayGetID(aPlant));
			for (int i = 0; i < mPierceCounter; i++)
			{
				if (mHitPlants[i] == aPlantID)
				{
					alreadyHit = true;
					break;
				}
			}
			if (alreadyHit)
			{
				continue; // Skip this plant, we've already hit it
			}
		}

		Rect aPlantRect = aPlant->GetPlantRect();
		if (GetRectOverlap(aProjectileRect, aPlantRect) > 8)
		{
			if (mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PEA || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_SNOWPEA || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PUFF)
			{
				return mBoard->GetTopPlantAt(aPlant->mPlantCol, aPlant->mRow, PlantPriority::TOPPLANT_EATING_ORDER);
			}
			else
			{
				return mBoard->GetTopPlantAt(aPlant->mPlantCol, aPlant->mRow, PlantPriority::TOPPLANT_CATAPULT_ORDER);
			}
		}
	}

	return nullptr;
}

bool Projectile::PeaAboutToHitTorchwood()
{
	if (mMotionType != ProjectileMotion::MOTION_STRAIGHT)
		return false;

	if (mProjectileType != ProjectileType::PROJECTILE_PEA && mProjectileType != ProjectileType::PROJECTILE_SNOWPEA)
		return false;

	Plant* aPlant = nullptr;
	while (mBoard->IteratePlants(aPlant))
	{
		if (aPlant->mSeedType == SeedType::SEED_TORCHWOOD && aPlant->mRow == mRow && !aPlant->NotOnGround() && mHitTorchwoodGridX != aPlant->mPlantCol)
		{
			Rect aPlantAttackRect = aPlant->GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);
			Rect aProjectileRect = GetProjectileRect();
			aProjectileRect.mX += 40;

			if (GetRectOverlap(aPlantAttackRect, aProjectileRect) > 10)
			{
				return true;
			}
		}
	}

	return false;
}

Zombie* Projectile::FindCollisionTarget()
{
	if (PeaAboutToHitTorchwood())  
		return nullptr;

	if (mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_SPIKE)
		return nullptr;

	Rect aProjectileRect = GetProjectileRect();
	Zombie* aBestZombie = nullptr;
	int aMinX = 0;

	Zombie* aZombie = nullptr;
	while (mBoard->IterateZombies(aZombie))
	{
		if ((aZombie->mZombieType == ZombieType::ZOMBIE_BOSS || aZombie->mRow == mRow) && aZombie->EffectedByDamage((unsigned int)mDamageRangeFlags))
		{
			if (aZombie->mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL && mPosZ >= 45.0f)
			{
				continue;
			}

			if (mProjectileType == ProjectileType::PROJECTILE_STAR && mProjectileAge < 25 && mVelX >= 0.0f && aZombie->mZombieType == ZombieType::ZOMBIE_DIGGER)
			{
				continue;
			}

			Rect aZombieRect = aZombie->GetZombieRect();
			if (GetRectOverlap(aProjectileRect, aZombieRect) > 0)
			{
				// Check if we've already hit this zombie
				bool alreadyHit = false;
				ZombieID aZombieID = mBoard->ZombieGetID(aZombie);
				for (int i = 0; i < mPierceCounter; i++)
				{
					if (mHitZombies[i] == aZombieID)
					{
						alreadyHit = true;
						break;
					}
				}

				// If not already hit, consider it a valid target
				if (!alreadyHit)
				{
					if (aBestZombie == nullptr || aZombie->mX < aMinX)
					{
						aBestZombie = aZombie;
						aMinX = aZombie->mX;
					}
				}
			}
		}
	}

	return aBestZombie;
}

void Projectile::CheckForCollision()
{
	if (mMotionType == ProjectileMotion::MOTION_PUFF && mProjectileAge >= 75)
	{
		Die();
		return;
	}

	if (mPosX > WIDE_BOARD_WIDTH || mPosX + mWidth < 0.0f)
	{
		if (mProjectileType == PROJECTILE_BOUNCING_PEA && !mHasBounced)
		{
			mHasBounced = true;
			mVelX *= -1; // Reverse horizontal direction
			mRotation = 3.14159f; // Make it face the other way
			return; // Don't die yet
		}
		Die();
		return;
	}

	if (mMotionType == ProjectileMotion::MOTION_HOMING)
	{
		Zombie* aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
		if (aZombie && aZombie->EffectedByDamage((unsigned int)mDamageRangeFlags))
		{
			Rect aProjectileRect = GetProjectileRect();
			Rect aZombieRect = aZombie->GetZombieRect();
			if (GetRectOverlap(aProjectileRect, aZombieRect) >= 0 && mPosY > aZombieRect.mY&& mPosY < aZombieRect.mY + aZombieRect.mHeight)
			{
				DoImpact(aZombie);
			}
		}
		return;
	}

	if (mProjectileType == ProjectileType::PROJECTILE_STAR && (mPosY > 600.0f || mPosY < 0.0f))
	{
		Die();
		return;
	}

	if ((mProjectileType == ProjectileType::PROJECTILE_PEA || mProjectileType == ProjectileType::PROJECTILE_STAR) && mShadowY - mPosY > 90.0f)
	{
		return;
	}

	if (mMotionType == ProjectileMotion::MOTION_FLOAT_OVER)
	{
		return;
	}

	if (mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PEA || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_SNOWPEA ||
		mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PUFF || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_SPIKE ||
		mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_STAR || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_KERNEL ||
		mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_BUTTER)
	{
		Plant* aPlant = FindCollisionTargetPlant();
		if (aPlant)
		{
			const ProjectileDefinition& aProjectileDef = GetProjectileDef();
			aPlant->mPlantHealth -= aProjectileDef.mDamage;
			aPlant->mEatenFlashCountdown = max(aPlant->mEatenFlashCountdown, 25);

			mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
			if (mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PEA)
			{
				mApp->AddTodParticle(mPosX - 3.0f, mPosY + 17.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_PEA_SPLAT);
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_SNOWPEA)
			{
				if (aPlant->mSeedType != SEED_SNOWPEA &&
					aPlant->mSeedType != SEED_WINTERMELON &&
					aPlant->mSeedType != SEED_ICEPEA &&
					aPlant->mSeedType != SEED_ICESHROOM &&
					aPlant->mSeedType != SEED_INSTANT_COFFEE &&
					aPlant->mSeedType != SEED_FIRESHOOTER &&
					aPlant->mSeedType != SEED_TORCHWOOD &&
					aPlant->mSeedType != SEED_BLUE_TORCHWOOD &&
					aPlant->mSeedType != SEED_ICE_PLANTERN)
				{
					aPlant->mChilledCounter = 1000;
				}
				mApp->AddTodParticle(mPosX - 3.0f, mPosY + 17.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_SNOWPEA_SPLAT);
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PUFF)
			{
				mApp->AddTodParticle(mPosX - 3.0f, mPosY + 17.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_PUFF_SPLAT);
			}
			else if (mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_SPIKE || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_STAR)
			{
				Plant* aPlant = FindCollisionTargetPlant();
				if (aPlant)
				{
					DoImpact(nullptr);
				}
				return;
			}
			else if (mProjectileType == mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_CABBAGE || mProjectileType == mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_KERNEL || 
					mProjectileType == mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_BUTTER)
			{
				return; // Lobbed projectiles are handled in UpdateLobMotion
			}
			Die();
		}
		return;
	}

	Zombie* aZombie = FindCollisionTarget();
	if (aZombie)
	{
		if (aZombie->mOnHighGround && CantHitHighGround())
		{
			return;
		}

		DoImpact(aZombie);
	}
}

bool Projectile::CantHitHighGround()
{
	if (mMotionType == ProjectileMotion::MOTION_BACKWARDS || mMotionType == ProjectileMotion::MOTION_HOMING)
		return false;

	return (
		mProjectileType == ProjectileType::PROJECTILE_PEA ||
		mProjectileType == ProjectileType::PROJECTILE_SNOWPEA ||
		mProjectileType == ProjectileType::PROJECTILE_STAR ||
		mProjectileType == ProjectileType::PROJECTILE_PUFF ||
		mProjectileType == ProjectileType::PROJECTILE_FIREBALL
		) && !mOnHighGround;
}

void Projectile::CheckForHighGround()
{
	float aShadowDelta = mShadowY - mPosY;

	if (mProjectileType == ProjectileType::PROJECTILE_PEA ||
		mProjectileType == ProjectileType::PROJECTILE_SNOWPEA ||
		mProjectileType == ProjectileType::PROJECTILE_FIREBALL ||
		mProjectileType == ProjectileType::PROJECTILE_SPIKE ||
		mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		if (aShadowDelta < 28.0f)
		{
			DoImpact(nullptr);
			return;
		}
	}

	if (mProjectileType == ProjectileType::PROJECTILE_PUFF && aShadowDelta < 0.0f)
	{
		DoImpact(nullptr);
		return;
	}

	if (mProjectileType == ProjectileType::PROJECTILE_STAR && aShadowDelta < 23.0f)
	{
		DoImpact(nullptr);
		return;
	}

	if (CantHitHighGround())
	{
		int aGridX = mBoard->PixelToGridXKeepOnBoard(mPosX + 30, mPosY);
		if (mBoard->mGridSquareType[aGridX][mRow] == GridSquareType::GRIDSQUARE_HIGH_GROUND)
		{
			DoImpact(nullptr);
		}
	}
}

bool Projectile::IsSplashDamage(Zombie* theZombie)
{
	if (mProjectileType && theZombie && theZombie->IsFireResistant())
		return false;

	return 
		mProjectileType == ProjectileType::PROJECTILE_MELON || 
		mProjectileType == ProjectileType::PROJECTILE_WINTERMELON || 
		mProjectileType == ProjectileType::PROJECTILE_FIREBALL ||
		mProjectileType == ProjectileType::PROJECTILE_RED_FIRE_PEA ||
		mProjectileType == ProjectileType::PROJECTILE_BLUE_FIRE_PEA ||
		mProjectileType == ProjectileType::PROJECTILE_WHITE_FIRE_PEA ||
		mProjectileType == ProjectileType::PROJECTILE_BIG_POISON_CABBAGE;
}

unsigned int Projectile::GetDamageFlags(Zombie* theZombie)
{
	unsigned int aDamageFlags = 0U;

	if (IsSplashDamage(theZombie))
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_HITS_SHIELD_AND_BODY, true);
	}
	else if (mMotionType == ProjectileMotion::MOTION_LOBBED || mMotionType == ProjectileMotion::MOTION_BACKWARDS)
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_BYPASSES_SHIELD, true);
	}
	else if (mMotionType == ProjectileMotion::MOTION_STAR && mVelX < 0.0f)
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_BYPASSES_SHIELD, true);
	}

	if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL || 
		mProjectileType == ProjectileType::PROJECTILE_RED_FIRE_PEA || 
		mProjectileType == ProjectileType::PROJECTILE_BLUE_FIRE_PEA ||
		mProjectileType == ProjectileType::PROJECTILE_WHITE_FIRE_PEA
		)
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_FIRE, true);
	}

	if (mProjectileType == ProjectileType::PROJECTILE_SNOWPEA ||
		mProjectileType == ProjectileType::PROJECTILE_WINTERMELON ||
		mProjectileType == ProjectileType::PROJECTILE_BLUE_FIRE_PEA ||
		mProjectileType == ProjectileType::PROJECTILE_ICEPEA
		)
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_FREEZE, true);
	}

	return aDamageFlags;
}

bool Projectile::IsZombieHitBySplash(Zombie* theZombie)
{
	Rect aProjectileRect = GetProjectileRect();
	if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
	{
		aProjectileRect.mWidth = 100;
	}

	int aRowDeviation = theZombie->mRow - mRow;
	Rect aZombieRect = theZombie->GetZombieRect();
	if (theZombie->IsFireResistant() && mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
	{
		return false;
	}

	if (theZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
	{
		aRowDeviation = 0;
	}
	if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
	{
		if (aRowDeviation != 0)
		{
			return false;
		}
	}
	else if (aRowDeviation > 1 || aRowDeviation < -1)
	{
		return false;
	}

	return theZombie->EffectedByDamage((unsigned int)mDamageRangeFlags) && GetRectOverlap(aProjectileRect, aZombieRect) >= 0;
}

void Projectile::DoSplashDamage(Zombie* theZombie)
{
	const ProjectileDefinition& aProjectileDef = GetProjectileDef();

	int aZombiesGetSplashed = 0;
	Zombie* aZombie = nullptr;
	while (mBoard->IterateZombies(aZombie))
	{
		if (aZombie != theZombie && IsZombieHitBySplash(aZombie))
		{
			aZombiesGetSplashed++;
		}
	}

	int aOriginalDamage = aProjectileDef.mDamage;
	int aSplashDamage = aProjectileDef.mDamage / 3;
	int aMaxSplashDamageAmount = aSplashDamage * 7;
	if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
	{
		aMaxSplashDamageAmount = aOriginalDamage;
	}
	int aSplashDamageAmount = aSplashDamage * aZombiesGetSplashed;
	if (aSplashDamageAmount > aMaxSplashDamageAmount)
	{
		//aSplashDamage *= aMaxSplashDamageAmount / aSplashDamage;
		aSplashDamage = aOriginalDamage * aMaxSplashDamageAmount / (aSplashDamageAmount * 3);
		aSplashDamage = max(aSplashDamage, 1);
	}

	aZombie = nullptr;
	while (mBoard->IterateZombies(aZombie))
	{
		if (IsZombieHitBySplash(aZombie))
		{
			if (mProjectileType == PROJECTILE_BIG_POISON_CABBAGE)
			{
				aZombie->ApplyPoison();
			}

			unsigned int aDamageFlags = GetDamageFlags(aZombie);
			if (aZombie == theZombie)
			{
				aZombie->TakeDamage(aOriginalDamage, aDamageFlags);
			}
			else
			{
				aZombie->TakeDamage(aSplashDamage, aDamageFlags);
			}
		}
	}
}

void Projectile::UpdateLobMotion()
{
	if (mProjectileType == ProjectileType::PROJECTILE_COBBIG && mPosZ < -700.0f)
	{
		mVelZ = 8.0f;
		mRow = mCobTargetRow;
		mPosX = mCobTargetX;
		int aCobTargetCol = mBoard->PixelToGridXKeepOnBoard(mCobTargetX, 0);
		mPosY = mBoard->GridToPixelY(aCobTargetCol, mCobTargetRow);
		mShadowY = mPosY + 67.0f;
		mRotation = -PI / 2;
	}

	mVelZ += mAccZ;
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_HIGH_GRAVITY)
	{
		mVelZ += mAccZ;
	}
	mPosX += mVelX;
	mPosY += mVelY;
	mPosZ += mVelZ;

	bool isRising = mVelZ < 0.0f;
	if (isRising && (mProjectileType == ProjectileType::PROJECTILE_BASKETBALL || mProjectileType == ProjectileType::PROJECTILE_COBBIG))
	{
		return;
	}
	if (mProjectileAge > 20)
	{
		if (isRising)
		{
			return;
		}

		float aMinCollisionZ = 0.0f;
		if (mProjectileType == ProjectileType::PROJECTILE_BUTTER)
		{
			aMinCollisionZ = -32.0f;
		}
		else if (mProjectileType == ProjectileType::PROJECTILE_BASKETBALL)
		{
			aMinCollisionZ = 60.0f;
		}
		else if (mProjectileType == ProjectileType::PROJECTILE_MELON || mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
		{
			aMinCollisionZ = -35.0f;
		}
		else if (mProjectileType == ProjectileType::PROJECTILE_CABBAGE || mProjectileType == ProjectileType::PROJECTILE_KERNEL)
		{
			aMinCollisionZ = -30.0f;
		}
		else if (mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_CABBAGE || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_KERNEL || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_BUTTER)
		{
			aMinCollisionZ = 30.0f;
		}
		else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
		{
			aMinCollisionZ = -60.0f;
		}
		if (mBoard->mPlantRow[mRow] == PlantRowType::PLANTROW_POOL)
		{
			aMinCollisionZ += 40.0f;
		}

		if (mPosZ <= aMinCollisionZ)
		{
			return;
		}
	}

	Plant* aPlant = nullptr;
	Zombie* aZombie = nullptr;
	if (mProjectileType == ProjectileType::PROJECTILE_BASKETBALL || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PEA || 
		mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PUFF || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_SNOWPEA ||
		mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_STAR || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_CABBAGE || 
		mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_KERNEL || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_BUTTER)
	{
		aPlant = FindCollisionTargetPlant();
	}
	else
	{
		aZombie = FindCollisionTarget();
	}

	float aGroundZ = 80.0f;
	if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		aGroundZ = -40.0f;
	}
	bool hitGround = mPosZ > aGroundZ;
	if (aZombie == nullptr && aPlant == nullptr && !hitGround)
	{
		return;
	}

	if (aPlant)
	{
		Plant* aUmbrellaPlant = mBoard->FindUmbrellaPlant(aPlant->mPlantCol, aPlant->mRow);
		if (aUmbrellaPlant)
		{
			if (aUmbrellaPlant->mState == PlantState::STATE_UMBRELLA_REFLECTING)
			{
				mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
				int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_TOP, 0, 1);
				mApp->AddTodParticle(mPosX + 20.0f, mPosY + 20.0f, aRenderPosition, ParticleEffect::PARTICLE_UMBRELLA_REFLECT);
				Die();
			}
			else if (aUmbrellaPlant->mState != PlantState::STATE_UMBRELLA_TRIGGERED)
			{
				mApp->PlayFoley(FoleyType::FOLEY_UMBRELLA);
				aUmbrellaPlant->DoSpecial();
			}
		}
		else
		{
			if (mProjectileType == PROJECTILE_ZOMBIE_BUTTER)
			{
				// Immune plants check
				if (aPlant->mSeedType != SEED_COBCANNON && aPlant->mSeedType != SEED_KERNELPULT &&
					aPlant->mSeedType != SEED_TORCHWOOD && aPlant->mSeedType != SEED_FIRESHOOTER &&
					aPlant->mSeedType != SEED_UMBRELLA)
				{
					aPlant->mButteredCounter = 400;

					int aButterOffsetX, aButterOffsetY;
					aPlant->GetButterSplatOffset(aButterOffsetX, aButterOffsetY);

					aPlant->mButterX = aButterOffsetX;
					aPlant->mButterY = aButterOffsetY;
				}
			}

			aPlant->mPlantHealth -= GetProjectileDef().mDamage;
			aPlant->mEatenFlashCountdown = max(aPlant->mEatenFlashCountdown, 25);
			mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
			Die();
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		if (GetGargantuars(mRow, mPosX + 80, mPosY + 40, 115, 1)){
			mBoard->mGargantuarsKilled++;
			if (mBoard->mGargantuarsKilled >= 2 && !mApp->mPlayedQuickplay)
				mApp->GetAchievement(ACHIEVEMENT_POPCORN_PARTY);
		}
		mBoard->KillAllZombiesInRadius(mRow, mPosX + 80, mPosY + 40, 115, 1, true, mDamageRangeFlags);
		DoImpact(nullptr);
	}
	else
	{
		DoImpact(aZombie);
	}
}

void Projectile::UpdateNormalMotion()
{
	if (mMotionType == ProjectileMotion::MOTION_BACKWARDS)
	{
		mPosX -= 3.33f;
		if (mProjectileType == PROJECTILE_BOUNCING_PEA && mPosX <= 0)
		{
			mMotionType = ProjectileMotion::MOTION_STRAIGHT;
			mApp->PlayFoley(FOLEY_BONK); // Make a bounce sound
		}
	}
	else if (mMotionType == ProjectileMotion::MOTION_HOMING)
	{
		Zombie* aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
		if (aZombie && aZombie->EffectedByDamage((unsigned int)mDamageRangeFlags))
		{
			Rect aZombieRect = aZombie->GetZombieRect();
			SexyVector2 aTargetCenter(aZombie->ZombieTargetLeadX(0.0f), aZombieRect.mY + aZombieRect.mHeight / 2);
			SexyVector2 aProjectileCenter(mPosX + mWidth / 2, mPosY + mHeight / 2);
			SexyVector2 aToTarget = (aTargetCenter - aProjectileCenter).Normalize();
			SexyVector2 aMotion(mVelX, mVelY);

			aMotion += aToTarget * (0.001f * mProjectileAge);
			aMotion = aMotion.Normalize();
			aMotion *= 2.0f;

			mVelX = aMotion.x;
			mVelY = aMotion.y;
			mRotation = -atan2(mVelY, mVelX);
		}

		mPosY += mVelY;
		mPosX += mVelX;
		mShadowY += mVelY;
		mRow = mBoard->PixelToGridYKeepOnBoard(mPosX, mPosY);
	}
	else if (mMotionType == ProjectileMotion::MOTION_STAR)
	{
		mPosY += mVelY;
		mPosX += mVelX;
		mShadowY += mVelY;

		if (mVelY != 0.0f)
		{
			mRow = mBoard->PixelToGridYKeepOnBoard(mPosX, mPosY);
		}
	}
	else if (mMotionType == ProjectileMotion::MOTION_BEE)
	{
		if (mProjectileAge < 60)
		{
			mPosY -= 0.5f;
		}
		mPosX += 3.33f;
	}
	else if (mMotionType == ProjectileMotion::MOTION_FLOAT_OVER)
	{
		if (mVelZ < 0.0f)
		{
			mVelZ += 0.002f;
			mVelZ = min(mVelZ, 0.0f);
			mPosY += mVelZ;
			mRotation = 0.3f - 0.7f * mVelZ * PI * 0.25f;
		}
		mPosX += 0.4f;
	}
	else if (mMotionType == ProjectileMotion::MOTION_BEE_BACKWARDS)
	{
		if (mProjectileAge < 60)
		{
			mPosY -= 0.5f;
		}
		mPosX -= 3.33f;
	}
	else if (mMotionType == ProjectileMotion::MOTION_THREEPEATER)
	{
		mPosX += 3.33f;
		mPosY += mVelY;
		mVelY *= 0.97f;
		mShadowY += mVelY;
	}
	else if (mMotionType == ProjectileMotion::MOTION_SPREAD)
	{
		mPosX += mVelX;
		mPosY += mVelY;
		mShadowY += mVelY;
		mRow = mBoard->PixelToGridYKeepOnBoard(mPosX, mPosY);
	}
	else if (mMotionType == ProjectileMotion::MOTION_PUFF)
	{
		mPosX += 6.66f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PUFF || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_SPIKE)
	{
		mPosX -= 6.66f; // Move left
	}
	else if (mMotionType == ProjectileMotion::MOTION_BOUNCE)
	{
		mPosX += mVelX;
		mPosY += mVelY;
		mShadowY += mVelY;
		mRow = mBoard->PixelToGridYKeepOnBoard(mPosX, mPosY);
		if ((mPosY < LAWN_YMIN && mVelY < 0) || (mPosY > 520 && mVelY > 0))
		{
			if (!mHasBounced)
			{
				mHasBounced = true;
				mVelY *= -1;
				mApp->PlaySample(SOUND_BONK);
			}
		}
	}
	else
	{
		if (mProjectileType == PROJECTILE_BOUNCING_PEA)
		{
			mPosX += mVelX;
		}
		else
		{
			mPosX += 3.33f;
		}
	}

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_HIGH_GRAVITY)
	{
		if (mMotionType == ProjectileMotion::MOTION_FLOAT_OVER)
		{
			mVelZ += 0.004f;
		}
		else
		{
			mVelZ += 0.2f;
		}

		mPosY += mVelZ;
	}

	CheckForCollision();
	CheckForHighGround();
}

void Projectile::UpdateMotion()
{
	if (mAnimTicksPerFrame > 0)
	{
		mAnimCounter = (mAnimCounter + 1) % (mNumFrames * mAnimTicksPerFrame);
		mFrame = mAnimCounter / mAnimTicksPerFrame;
	}

	int aOldRow = mRow;
	float aOldY = mBoard->GetPosYBasedOnRow(mPosX, mRow);
	if (mMotionType == ProjectileMotion::MOTION_LOBBED)
	{
		UpdateLobMotion();
	}
	else
	{
		UpdateNormalMotion();
	}

	float aSlopeHeightChange = mBoard->GetPosYBasedOnRow(mPosX, aOldRow) - aOldY;
	if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		aSlopeHeightChange = 0.0f;  
	}
	if (mMotionType == ProjectileMotion::MOTION_FLOAT_OVER)
	{
		mPosY += aSlopeHeightChange;
	}
	if (mMotionType == ProjectileMotion::MOTION_LOBBED)
	{
		mPosY += aSlopeHeightChange;
		mPosZ -= aSlopeHeightChange;
	}
	mShadowY += aSlopeHeightChange;
	mX = (int)mPosX;
	mY = (int)(mPosY + mPosZ);
}

void Projectile::PlayImpactSound(Zombie* theZombie)
{
	bool aPlayHelmSound = true;
	bool aPlaySplatSound = true;
	if (mProjectileType == ProjectileType::PROJECTILE_KERNEL)
	{
		mApp->PlayFoley(FoleyType::FOLEY_KERNEL_SPLAT);
		aPlayHelmSound = false;
		aPlaySplatSound = false;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BUTTER)
	{
		mApp->PlayFoley(FoleyType::FOLEY_BUTTER);
		aPlaySplatSound = false;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL && IsSplashDamage(theZombie))
	{
		mApp->PlayFoley(FoleyType::FOLEY_IGNITE);
		aPlayHelmSound = false;
		aPlaySplatSound = false;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_MELON || mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
	{
		mApp->PlayFoley(FoleyType::FOLEY_MELONIMPACT);
		aPlaySplatSound = false;
	}

	if (aPlayHelmSound && theZombie)
	{
		if (theZombie->mHelmType == HELMTYPE_PAIL)
		{
			mApp->PlayFoley(FoleyType::FOLEY_SHIELD_HIT);
			aPlaySplatSound = false;
		}
		else if (theZombie->mHelmType == HELMTYPE_TRAFFIC_CONE || theZombie->mHelmType == HELMTYPE_DIGGER || theZombie->mHelmType == HELMTYPE_FOOTBALL)
		{
			mApp->PlayFoley(FoleyType::FOLEY_PLASTIC_HIT);
		}
	}

	if (aPlaySplatSound)
	{
		mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
	}
}

void Projectile::DoImpact(Zombie* theZombie)
{
	PlayImpactSound(theZombie);

	if (mProjectileType == PROJECTILE_ZOMBIE_SPIKE)
	{
		Plant* aPlant = FindCollisionTargetPlant();
		if (aPlant)
		{
			mApp->PlayFoley(FOLEY_SPLAT);
			aPlant->TakeDamage(GetProjectileDef().mDamage, 0U);

			if (mPierceCounter < 10)
			{
				mHitPlants[mPierceCounter] = static_cast<PlantID>(mBoard->mPlants.DataArrayGetID(aPlant));
			}
			mPierceCounter++;

			if (mPierceCounter >= 2) // Dies after hitting 2 plants
			{
				Die();
			}
		}
		return;
	}

	if (mProjectileType == PROJECTILE_BIG_PEA || mProjectileType == PROJECTILE_BIG_FIREPEA)
	{
		if (Rand(2) == 0) // 50% chance
		{
			theZombie->mPosX += 80; // Push back one tile
		}
		else
		{
			theZombie->Stun(50); // Stun for 0.5 seconds (50 ticks)
		}
	}

	if (mProjectileType == PROJECTILE_BIG_BUTTER)
	{
		if (theZombie) // Make sure there was an initial target
		{
			// Get the grid cell of the zombie that was hit
			int aCenterCol = mBoard->PixelToGridX(theZombie->mX, theZombie->mY);
			int aCenterRow = theZombie->mRow;

			// Iterate through all zombies on the board
			Zombie* aSplashTarget = nullptr;
			while (mBoard->IterateZombies(aSplashTarget))
			{
				// Check if the zombie is within the 3x3 area
				if (abs(aSplashTarget->mRow - aCenterRow) <= 1 &&
					abs(mBoard->PixelToGridX(aSplashTarget->mX, aSplashTarget->mY) - aCenterCol) <= 1)
				{
					aSplashTarget->ApplyButter();
				}
			}
		}
	}

	if (IsSplashDamage(theZombie))
	{
		if ((mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_RED_FIRE_PEA || mProjectileType == ProjectileType::PROJECTILE_WHITE_FIRE_PEA || mProjectileType == ProjectileType::PROJECTILE_BIG_FIREPEA) && theZombie)
		{
			theZombie->RemoveColdEffects();
		}

		DoSplashDamage(theZombie);
	}
	else if (theZombie)
	{
		unsigned int aDamageFlags = GetDamageFlags(theZombie);
		theZombie->TakeDamage(GetProjectileDef().mDamage, aDamageFlags);
	}

	if (mProjectileType == PROJECTILE_ICEPEA)
	{
		theZombie->HitIceTrap();
	}

	float aLastPosX = mPosX - mVelX;
	float aLastPosY = mPosY + mPosZ - mVelY - mVelZ;
	ParticleEffect aEffect = ParticleEffect::PARTICLE_NONE;
	float aSplatPosX = mPosX + 12.0f;
	float aSplatPosY = mPosY + 12.0f;
	if (mProjectileType == ProjectileType::PROJECTILE_MELON)
	{
		mApp->AddTodParticle(aLastPosX + 30.0f, aLastPosY + 30.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_MELONSPLASH);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
	{
		mApp->AddTodParticle(aLastPosX + 30.0f, aLastPosY + 30.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_WINTERMELON);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BIG_POISON_CABBAGE)
	{
		mApp->AddTodParticle(aLastPosX + 30.0f, aLastPosY + 30.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_CABBAGE_SPLAT);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		int aRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_GROUND, mCobTargetRow, 2);
		mApp->AddTodParticle(mPosX + 80.0f, mPosY + 40.0f, aRenderOrder, ParticleEffect::PARTICLE_BLASTMARK);
		mApp->AddTodParticle(mPosX + 80.0f, mPosY + 40.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_POPCORNSPLASH);
		mApp->PlaySample(SOUND_DOOMSHROOM);
		mBoard->ShakeBoard(3, -4);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PEA)
	{
		aSplatPosX -= 15.0f;
		aEffect = ParticleEffect::PARTICLE_PEA_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
	{
		aSplatPosX -= 15.0f;
		aEffect = ParticleEffect::PARTICLE_SNOWPEA_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
	{
		if (IsSplashDamage(theZombie))
		{
			if ((mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_RED_FIRE_PEA) && theZombie)
			{
				theZombie->RemoveColdEffects();
			}
			DoSplashDamage(theZombie);
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_RED_FIRE_PEA)
	{
		// Remove freezing/slowing effects from the direct target
		if (theZombie)
		{
			theZombie->RemoveColdEffects();
		}

		// Play fire sound and create a fire explosion animation
		mApp->PlayFoley(FoleyType::FOLEY_IGNITE);
		Reanimation* aFireReanim = mApp->AddReanimation(mPosX + 38.0f, mPosY - 20.0f, mRenderOrder + 1, ReanimationType::REANIM_JALAPENO_FIRE);
		aFireReanim->mAnimTime = 0.25f;
		aFireReanim->mAnimRate = 24.0f;
		aFireReanim->OverrideScale(0.7f, 0.4f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BLUE_FIRE_PEA)
	{
		// Play frozen sound and create a blue splat particle
		mApp->PlayFoley(FoleyType::FOLEY_FROZEN);
		aSplatPosX -= 15.0f;
		aEffect = ParticleEffect::PARTICLE_SNOWPEA_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_WHITE_FIRE_PEA)
	{
		if (IsSplashDamage(theZombie))
		{
			Reanimation* aFireReanim = mApp->AddReanimation(mPosX + 38.0f, mPosY - 20.0f, mRenderOrder + 1, ReanimationType::REANIM_JALAPENO_FIRE);
			aFireReanim->mAnimTime = 0.25f;
			aFireReanim->mAnimRate = 24.0f;
			aFireReanim->OverrideScale(0.7f, 0.4f);
			DoSplashDamage(theZombie);
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BLACK_FIRE_PEA)
	{
		if (IsSplashDamage(theZombie))
		{
			Reanimation* aFireReanim = mApp->AddReanimation(mPosX + 38.0f, mPosY - 20.0f, mRenderOrder + 1, ReanimationType::REANIM_JALAPENO_FIRE);
			aFireReanim->mAnimTime = 0.25f;
			aFireReanim->mAnimRate = 24.0f;
			aFireReanim->OverrideScale(0.7f, 0.4f);
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_STAR)
	{
		aEffect = ParticleEffect::PARTICLE_STAR_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PUFF || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PUFF)
	{
		aSplatPosX -= 20.0f;
		aEffect = ParticleEffect::PARTICLE_PUFF_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_CABBAGE)
	{
		aSplatPosX = aLastPosX - 38.0f;
		aSplatPosY = aLastPosY + 23.0f;
		aEffect = ParticleEffect::PARTICLE_CABBAGE_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BUTTER)
	{
		aSplatPosX = aLastPosX - 20.0f;
		aSplatPosY = aLastPosY + 63.0f;
		aEffect = ParticleEffect::PARTICLE_BUTTER_SPLAT;

		if (theZombie)
		{
			theZombie->ApplyButter();
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_BUTTER)
	{
		Plant* aPlant = FindCollisionTargetPlant();
		if (aPlant)
		{
			if (aPlant->mSeedType != SEED_COBCANNON && aPlant->mSeedType != SEED_KERNELPULT &&
				aPlant->mSeedType != SEED_TORCHWOOD && aPlant->mSeedType != SEED_FIRESHOOTER &&
				aPlant->mSeedType != SEED_UMBRELLA)
			{
				aPlant->mButteredCounter = 400; // Butter for 4 seconds
			}
		}
	}

	if (mProjectileType == ProjectileType::PROJECTILE_BIG_POISON_CABBAGE && theZombie)
	{
		theZombie->ApplyPoison();
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_POISON_CABBAGE && theZombie)
	{
		theZombie->ApplyPoison();
	}

	Plant* aPlant = nullptr;
	if (mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_SNOWPEA && aPlant)
	{
		aPlant->mChilledCounter = 1000; // Chill the plant for 10 seconds
	}

	if (aEffect != ParticleEffect::PARTICLE_NONE)
	{
		if (theZombie)
		{
			float aPosX = aSplatPosX + 52.0f - theZombie->mX;
			float aPosY = aSplatPosY - theZombie->mY;
			if (theZombie->mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL || theZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING_IN_POOL)
			{
				aPosY += 60.0f;
			}
			if (mMotionType == ProjectileMotion::MOTION_BACKWARDS)
			{
				aPosX -= 80.0f;
			}
			else if (mPosX > theZombie->mX + 40 && mMotionType != ProjectileMotion::MOTION_LOBBED)
			{
				aPosX -= 60.0f;
			}

			aPosY = ClampFloat(aPosY, 20.0f, 100.0f);
			theZombie->AddAttachedParticle(aPosX, aPosY, aEffect);
		}
		else
		{
			mApp->AddTodParticle(aSplatPosX, aSplatPosY, mRenderOrder + 1, aEffect);
		}
	}

	if (mProjectileType == PROJECTILE_BLUE_SPIKE || mProjectileType == PROJECTILE_BLACK_SPIKE)
	{
		// Record the zombie it hit
		if (theZombie && mPierceCounter < 10)
		{
			mHitZombies[mPierceCounter] = mBoard->ZombieGetID(theZombie);
		}

		mPierceCounter++;

		int pierceLimit = (mProjectileType == PROJECTILE_BLACK_SPIKE) ? 10 : 3;
		if (mPierceCounter < pierceLimit)
		{
			return; // Survive to hit another zombie
		}
	}

	if (mProjectileType == ProjectileType::PROJECTILE_RED_STAR && theZombie)
	{
		theZombie->mIsOrange = true;
		theZombie->mOrangeEffectCountdown = 500; // 500 ticks = 5 seconds
	}

	Die();
}

void Projectile::Update()
{
	mProjectileAge++;
	if (mApp->mGameScene != GameScenes::SCENE_PLAYING && !mBoard->mCutScene->ShouldRunUpsellBoard())
		return;

	int aTime = 20;
	if (mProjectileType == ProjectileType::PROJECTILE_PEA || 
		mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || 
		mProjectileType == ProjectileType::PROJECTILE_CABBAGE || 
		mProjectileType == ProjectileType::PROJECTILE_MELON || 
		mProjectileType == ProjectileType::PROJECTILE_WINTERMELON || 
		mProjectileType == ProjectileType::PROJECTILE_KERNEL || 
		mProjectileType == ProjectileType::PROJECTILE_BUTTER || 
		mProjectileType == ProjectileType::PROJECTILE_COBBIG || 
		mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PEA || 
		mProjectileType == ProjectileType::PROJECTILE_SPIKE ||
		mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_SNOWPEA ||
		mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PUFF ||
		mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_SPIKE)
	{
		aTime = 0;
	}
	if (mProjectileAge > aTime)
	{
		mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PROJECTILE, mRow, 0);
	}

	if (mClickBackoffCounter > 0)
	{
		mClickBackoffCounter--;
	}
	mRotation += mRotationSpeed;

	UpdateMotion();
	AttachmentUpdateAndMove(mAttachmentID, mPosX, mPosY + mPosZ);
}

void Projectile::Draw(Graphics* g)
{
	const ProjectileDefinition& aProjectileDef = GetProjectileDef();

	Image* aImage = nullptr;
	float aScale = 1.0f;
	int aCelRow = 0;
	int aCelCol = 0;
	bool aMirror = false;

	if (mProjectileType == PROJECTILE_ARMOR)
	{
		aScale = 0.8f;
		switch (mArmorType)
		{
		case MAGNET_ITEM_PAIL_1: aImage = IMAGE_REANIM_ZOMBIE_BUCKET1; break;
		case MAGNET_ITEM_PAIL_2: aImage = IMAGE_REANIM_ZOMBIE_BUCKET2; break;
		case MAGNET_ITEM_PAIL_3: aImage = IMAGE_REANIM_ZOMBIE_BUCKET3; break;
		case MAGNET_ITEM_FOOTBALL_HELMET_1: aImage = IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET; break;
		case MAGNET_ITEM_FOOTBALL_HELMET_2: aImage = IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET2; break;
		case MAGNET_ITEM_FOOTBALL_HELMET_3: aImage = IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET3; break;
		case MAGNET_ITEM_DOOR_1: aImage = IMAGE_REANIM_ZOMBIE_SCREENDOOR1; break;
		case MAGNET_ITEM_DOOR_2: aImage = IMAGE_REANIM_ZOMBIE_SCREENDOOR2; break;
		case MAGNET_ITEM_DOOR_3: aImage = IMAGE_REANIM_ZOMBIE_SCREENDOOR3; break;
		case MAGNET_ITEM_BLACK_CONE_1: aImage = IMAGE_REANIM_ZOMBIE_BLACK_CONE1; break;
		case MAGNET_ITEM_BLACK_CONE_2: aImage = IMAGE_REANIM_ZOMBIE_BLACK_CONE2; break;
		case MAGNET_ITEM_BLACK_CONE_3: aImage = IMAGE_REANIM_ZOMBIE_BLACK_CONE3; break;
		case MAGNET_ITEM_BLACK_PAIL_1: aImage = IMAGE_REANIM_ZOMBIE_BLACK_BUCKET1; break;
		case MAGNET_ITEM_BLACK_PAIL_2: aImage = IMAGE_REANIM_ZOMBIE_BLACK_BUCKET2; break;
		case MAGNET_ITEM_BLACK_PAIL_3: aImage = IMAGE_REANIM_ZOMBIE_BLACK_BUCKET3; break;
		case MAGNET_ITEM_BLACK_DOOR_1: aImage = IMAGE_REANIM_ZOMBIE_BLACK_SCREENDOOR1; break;
		case MAGNET_ITEM_BLACK_DOOR_2: aImage = IMAGE_REANIM_ZOMBIE_BLACK_SCREENDOOR2; break;
		case MAGNET_ITEM_BLACK_DOOR_3: aImage = IMAGE_REANIM_ZOMBIE_BLACK_SCREENDOOR3; break;
		case MAGNET_ITEM_POGO_1: aImage = IMAGE_ZOMBIEPOGO; aCelCol = 0; break;
		case MAGNET_ITEM_POGO_2: aImage = IMAGE_ZOMBIEPOGO; aCelCol = 1; break;
		case MAGNET_ITEM_POGO_3: aImage = IMAGE_ZOMBIEPOGO; aCelCol = 2; break;
		case MAGNET_ITEM_JACK_IN_THE_BOX: aImage = IMAGE_REANIM_ZOMBIE_JACKBOX_BOX; break;
		case MAGNET_ITEM_LADDER_1: aImage = IMAGE_REANIM_ZOMBIE_LADDER_1; break;
		case MAGNET_ITEM_LADDER_2: aImage = IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE1; break;
		case MAGNET_ITEM_LADDER_3: aImage = IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE2; break;
		case MAGNET_ITEM_LADDER_PLACED: aImage = IMAGE_REANIM_ZOMBIE_LADDER_5; break;
		case MAGNET_ITEM_PICK_AXE: aImage = IMAGE_REANIM_ZOMBIE_DIGGER_PICKAXE; break;
		default: break;
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		aImage = IMAGE_REANIM_COBCANNON_COB;
		aScale = 0.9f;
		g->SetColor(Color::White);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PEA || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PEA || mProjectileType == ProjectileType::PROJECTILE_GRAPESHOT || mProjectileType == ProjectileType::PROJECTILE_BOUNCING_PEA || mProjectileType == ProjectileType::PROJECTILE_BIG_PEA)
	{
		aImage = IMAGE_PROJECTILEPEA;
		if (mProjectileType == PROJECTILE_BIG_PEA)
		{
			aScale = 2.0f;
			g->SetColor(Color::White);
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_SNOWPEA)
	{
		aImage = IMAGE_PROJECTILESNOWPEA;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
	{
		aImage = IMAGE_PROJECTILESNOWPEA;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_ICEPEA)
	{
		aImage = IMAGE_PROJECTILESNOWPEA;
		g->SetColorizeImages(true);
		g->SetColor(Color(100, 100, 255, 255));
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_RED_FIRE_PEA)
	{
		aImage = nullptr; // Hide the base pea image
		Reanimation* aFireReanim = FindReanimAttachment(mAttachmentID);
		if (aFireReanim)
		{
			aFireReanim->mColorOverride = Color(255, 100, 100, 255);
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BLUE_FIRE_PEA)
	{
		aImage = nullptr; // Hide the base pea image
		Reanimation* aFireReanim = FindReanimAttachment(mAttachmentID);
		if (aFireReanim)
		{
			aFireReanim->mColorOverride = Color(50, 50, 255, 255);
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_WHITE_FIRE_PEA)
	{
		aImage = nullptr; // Hide the base pea image
		Reanimation* aFireReanim = FindReanimAttachment(mAttachmentID);
		if (aFireReanim)
		{
			aFireReanim->mColorOverride = Color(255, 255, 255, 255);
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BLACK_FIRE_PEA)
	{
		aImage = nullptr; // Hide the base pea image
		Reanimation* aFireReanim = FindReanimAttachment(mAttachmentID);
		if (aFireReanim)
		{
			aFireReanim->mColorOverride = Color(10, 10, 10, 255);
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
	{
		aImage = nullptr;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BIG_FIREPEA)
	{
		aImage = nullptr;
		Reanimation* aFireReanim = FindReanimAttachment(mAttachmentID);
		if (aFireReanim)
		{
			aScale = 2.0f;
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SPIKE)
	{
		aImage = IMAGE_PROJECTILECACTUS;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_SPIKE)
	{
		aImage = IMAGE_PROJECTILECACTUS;
		aMirror = true;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BLUE_SPIKE)
	{
		aImage = IMAGE_PROJECTILECACTUS;
		g->SetColorizeImages(true);
		g->SetColor(Color(0, 100, 255, 255));
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BLACK_SPIKE)
	{
		aImage = IMAGE_PROJECTILECACTUS;
		g->SetColorizeImages(true);
		g->SetColor(Color(0, 0, 0, 255)); // black tint
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_STAR || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_STAR)
	{
		aImage = IMAGE_PROJECTILE_STAR;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_RED_STAR)
	{
		aImage = IMAGE_PROJECTILE_STAR;
		g->SetColorizeImages(true);
		g->SetColor(Color(255, 100, 100, 255)); // This gives it a red tint
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PUFF || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PUFF)
	{
		aImage = IMAGE_PUFFSHROOM_PUFF1;
		aScale = TodAnimateCurveFloat(0, 30, mProjectileAge, 0.3f, 1.0f, TodCurves::CURVE_LINEAR);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BASKETBALL)
	{
		aImage = IMAGE_REANIM_ZOMBIE_CATAPULT_BASKETBALL;
		aScale = 1.1f;
		g->SetColor(Color::White);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_POISON_CABBAGE)
	{
		aImage = IMAGE_REANIM_CABBAGEPULT_CABBAGE;
		aScale = 1.0f;
		g->SetColorizeImages(true);
		g->SetColor(Color(128, 0, 128, 255)); // Purple tint
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BIG_POISON_CABBAGE)
	{
		aImage = IMAGE_REANIM_CABBAGEPULT_CABBAGE;
		aScale = 2.0f; // twice as big
		g->SetColorizeImages(true);
		g->SetColor(Color(128, 0, 128, 255)); // Purple tint
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_CABBAGE || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_CABBAGE)
	{
		aImage = IMAGE_REANIM_CABBAGEPULT_CABBAGE;
		aScale = 1.0f;
		g->SetColor(Color::White);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_KERNEL || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_KERNEL)
	{
		aImage = IMAGE_REANIM_CORNPULT_KERNAL;
		aScale = 0.95f;
		g->SetColor(Color::White);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BUTTER || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_BUTTER)
	{
		aImage = IMAGE_REANIM_CORNPULT_BUTTER;
		aScale = 0.8f;
		g->SetColor(Color::White);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BIG_BUTTER)
	{
		aImage = IMAGE_REANIM_CORNPULT_BUTTER;
		aScale = 1.6f;
		g->SetColor(Color::White);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_MELON)
	{
		aImage = IMAGE_REANIM_MELONPULT_MELON;
		aScale = 1.0f;
		g->SetColor(Color::White);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
	{
		aImage = IMAGE_REANIM_WINTERMELON_PROJECTILE;
		aScale = 1.0f;
		g->SetColor(Color::White);
	}
	else
	{
		TOD_ASSERT();
	}

	if (mIsGrapeshot)
	{
		g->SetColorizeImages(true);
		g->SetColor(Color(255, 100, 100, 255)); // A nice red tint
	}

	if (mMotionType == ProjectileMotion::MOTION_BEE_BACKWARDS)
	{
		aMirror = true;
	}

	if (aImage)
	{
		TOD_ASSERT(aProjectileDef.mImageRow < aImage->mNumRows);
		TOD_ASSERT(mFrame < aImage->mNumCols);

		int aCelWidth = aImage->GetCelWidth();
		int aCelHeight = aImage->GetCelHeight();
		Rect aSrcRect(aCelWidth * mFrame, aCelHeight * aCelRow, aCelWidth, aCelHeight);
		if (FloatApproxEqual(mRotation, 0.0f) && FloatApproxEqual(aScale, 1.0f))
		{
			Rect aDestRect(0, 0, aCelWidth, aCelHeight);
			g->DrawImageMirror(aImage, aDestRect, aSrcRect, aMirror);
		}
		else
		{
			float aOffsetX = mPosX + aCelWidth * 0.5f;
			float aOffsetY = mPosZ + mPosY + aCelHeight * 0.5f;
			SexyTransform2D aTransform;
			TodScaleRotateTransformMatrix(aTransform, aOffsetX + mBoard->mX, aOffsetY + mBoard->mY, mRotation, aScale, aScale);
			TodBltMatrix(g, aImage, aTransform, g->mClipRect, g->GetColor(), g->mDrawMode, aSrcRect);
		}
	}

	if (mAttachmentID != AttachmentID::ATTACHMENTID_NULL)
	{
		Graphics theParticleGraphics(*g);
		MakeParentGraphicsFrame(&theParticleGraphics);
		AttachmentDraw(mAttachmentID, &theParticleGraphics, false);
	}

	if (mIsGrapeshot || mProjectileType == ProjectileType::PROJECTILE_ICEPEA ||
		mProjectileType == ProjectileType::PROJECTILE_POISON_CABBAGE ||
		mProjectileType == ProjectileType::PROJECTILE_BIG_POISON_CABBAGE ||
		mProjectileType == ProjectileType::PROJECTILE_BLUE_SPIKE ||
		mProjectileType == ProjectileType::PROJECTILE_BLACK_SPIKE ||
		mProjectileType == ProjectileType::PROJECTILE_RED_STAR)
	{
		g->SetColorizeImages(false);
	}
}

void Projectile::DrawShadow(Graphics* g)
{
	int aCelCol = 0;
	float aScale = 1.0f;
	float aStretch = 1.0f;
	float aOffsetX = mPosX - mX;
	float aOffsetY = mPosY - mY;

	int aGridX = mBoard->PixelToGridXKeepOnBoard(mX, mY);
	bool isHighGround = false;
	if (mBoard->mGridSquareType[aGridX][mRow] == GridSquareType::GRIDSQUARE_HIGH_GROUND)
	{
		isHighGround = true;
	}
	if (mOnHighGround && !isHighGround)
	{
		aOffsetY += HIGH_GROUND_HEIGHT;
	}
	else if (!mOnHighGround && isHighGround)
	{
		aOffsetY -= HIGH_GROUND_HEIGHT;
	}

	if (mBoard->StageIsNight())
	{
		aCelCol = 1;
	}

	switch (mProjectileType)
	{
	case ProjectileType::PROJECTILE_PEA:
	case ProjectileType::PROJECTILE_ZOMBIE_PEA:
		aOffsetX += 3.0f;
		break;

	case ProjectileType::PROJECTILE_SNOWPEA:
		aOffsetX += -1.0f;
		aScale = 1.3f;
		break;

	case ProjectileType::PROJECTILE_STAR:
		aOffsetX += 7.0f;
		break;

	case ProjectileType::PROJECTILE_CABBAGE:
	case ProjectileType::PROJECTILE_KERNEL:
	case ProjectileType::PROJECTILE_BUTTER:
	case ProjectileType::PROJECTILE_MELON:
	case ProjectileType::PROJECTILE_WINTERMELON:
		aOffsetX += 3.0f;
		aOffsetY += 10.0f;
		aScale = 1.6f;
		break;

	case ProjectileType::PROJECTILE_PUFF:
	case ProjectileType::PROJECTILE_ZOMBIE_PUFF:
		return;
		
	case ProjectileType::PROJECTILE_COBBIG:
		aScale = 1.0f;
		aStretch = 3.0f;
		aOffsetX += 57.0f;
		break;

	case ProjectileType::PROJECTILE_FIREBALL:
		aScale = 1.4f;
		break;

	case ProjectileType::PROJECTILE_GRAPESHOT:
		mFrame = 0;
		mNumFrames = 1;
		mIsGrapeshot = true; // Set our custom color flag to true
		break;
	}

	if (mMotionType == ProjectileMotion::MOTION_LOBBED)
	{
		float aHeight = ClampFloat(-mPosZ, 0.0f, 200.0f);
		aScale *= 200.0f / (aHeight + 200.0f);
	}

	TodDrawImageCelScaledF(g, IMAGE_PEA_SHADOWS, aOffsetX, (mShadowY - mPosY + aOffsetY), aCelCol, 0, aScale * aStretch, aScale);
}

void Projectile::Die()
{
	mDead = true;

	if (mProjectileType == ProjectileType::PROJECTILE_PUFF || mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
	{
		AttachmentCrossFade(mAttachmentID, "FadeOut");
		AttachmentDetach(mAttachmentID);
	}
	else
	{
		AttachmentDie(mAttachmentID);
	}
}

Rect Projectile::GetProjectileRect()
{
	if (mProjectileType == ProjectileType::PROJECTILE_PEA || 
		mProjectileType == ProjectileType::PROJECTILE_SNOWPEA ||
		mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PEA ||
		mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_SNOWPEA ||
		mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PUFF)
	{
		return Rect(mX - 15, mY, mWidth + 15, mHeight);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		return Rect(mX + mWidth / 2 - 115, mY + mHeight / 2 - 115, 230, 230);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_MELON || mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
	{
		return Rect(mX + 20, mY, 60, mHeight);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
	{
		return Rect(mX, mY, mWidth - 10, mHeight);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SPIKE || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_SPIKE)
	{
		return Rect(mX - 25, mY, mWidth + 25, mHeight);
	}
	else
	{
		return Rect(mX, mY, mWidth, mHeight);
	}
}

void Projectile::ConvertToFireball(int theGridX)
{
	if (mHitTorchwoodGridX == theGridX)
		return;

	mProjectileType = ProjectileType::PROJECTILE_FIREBALL;
	mHitTorchwoodGridX = theGridX;
	mApp->PlayFoley(FoleyType::FOLEY_FIREPEA);

	float aOffsetX = -25.0f;
	float aOffsetY = -25.0f;
	Reanimation* aFirePeaReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_FIRE_PEA);
	if (mMotionType == ProjectileMotion::MOTION_BACKWARDS)
	{
		aFirePeaReanim->OverrideScale(-1.0f, 1.0f);
		aOffsetX += 80.0f;
	}

	aFirePeaReanim->SetPosition(mPosX + aOffsetX, mPosY + aOffsetY);
	aFirePeaReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
	aFirePeaReanim->mAnimRate = RandRangeFloat(50.0f, 80.0f);
	AttachReanim(mAttachmentID, aFirePeaReanim, aOffsetX, aOffsetY);
}

void Projectile::ConvertToRedFirePea(int theGridX)
{
    if (mHitTorchwoodGridX == theGridX)
        return;

    mProjectileType = ProjectileType::PROJECTILE_RED_FIRE_PEA;
    mHitTorchwoodGridX = theGridX;
    mApp->PlayFoley(FoleyType::FOLEY_FIREPEA);
}

void Projectile::ConvertToBlueFirePea(int theGridX)
{
	if (mHitTorchwoodGridX == theGridX)
		return;

	mProjectileType = ProjectileType::PROJECTILE_BLUE_FIRE_PEA;
	mHitTorchwoodGridX = theGridX;
	mApp->PlayFoley(FoleyType::FOLEY_FROZEN);

	// If no fire animation is attached yet (i.e., coming from an Ice Pea), add one.
	if (FindReanimAttachment(mAttachmentID) == nullptr)
	{
		float aOffsetX = -25.0f;
		float aOffsetY = -25.0f;
		Reanimation* aFirePeaReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_FIRE_PEA);

		if (mMotionType == ProjectileMotion::MOTION_BACKWARDS)
		{
			aFirePeaReanim->OverrideScale(-1.0f, 1.0f);
			aOffsetX += 80.0f;
		}

		aFirePeaReanim->SetPosition(mPosX + aOffsetX, mPosY + aOffsetY);
		aFirePeaReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
		aFirePeaReanim->mAnimRate = RandRangeFloat(50.0f, 80.0f);
		AttachReanim(mAttachmentID, aFirePeaReanim, aOffsetX, aOffsetY);
	}
}

void Projectile::ConvertToWhiteFirePea(int theGridX)
{
	if (mHitTorchwoodGridX == theGridX)
		return;

	mProjectileType = ProjectileType::PROJECTILE_WHITE_FIRE_PEA;
	mHitTorchwoodGridX = theGridX;
	mApp->PlayFoley(FoleyType::FOLEY_IGNITE); // Play a fire sound

	// Find existing fire animation or create a new one
	Reanimation* aFireReanim = FindReanimAttachment(mAttachmentID);
	if (aFireReanim == nullptr)
	{
		float aOffsetX = -25.0f;
		float aOffsetY = -25.0f;
		aFireReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_FIRE_PEA);

		if (mMotionType == ProjectileMotion::MOTION_BACKWARDS)
		{
			aFireReanim->OverrideScale(-1.0f, 1.0f);
			aOffsetX += 80.0f;
		}

		aFireReanim->SetPosition(mPosX + aOffsetX, mPosY + aOffsetY);
		aFireReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
		aFireReanim->mAnimRate = RandRangeFloat(50.0f, 80.0f);
		AttachReanim(mAttachmentID, aFireReanim, aOffsetX, aOffsetY);
	}

	// Use white color and set a filter effect to convert orange to white
	aFireReanim->mColorOverride = Color(255, 255, 255, 255);
	aFireReanim->mFilterEffect = FilterEffect::FILTER_EFFECT_WHITE;

	// Create additional white overlay to enhance the white effect
	aFireReanim->mExtraOverlayColor = Color(200, 200, 200, 150);
}

void Projectile::ConvertToBlackFirePea(int theGridX)
{
	if (mHitTorchwoodGridX == theGridX)
		return;

	mProjectileType = ProjectileType::PROJECTILE_BLACK_FIRE_PEA;
	mHitTorchwoodGridX = theGridX;
	mApp->PlayFoley(FoleyType::FOLEY_IGNITE); // Play a fire sound

	// Find existing fire animation or create a new one
	Reanimation* aFireReanim = FindReanimAttachment(mAttachmentID);
	if (aFireReanim == nullptr)
	{
		float aOffsetX = -25.0f;
		float aOffsetY = -25.0f;
		aFireReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_FIRE_PEA);

		if (mMotionType == ProjectileMotion::MOTION_BACKWARDS)
		{
			aFireReanim->OverrideScale(-1.0f, 1.0f);
			aOffsetX += 80.0f;
		}

		aFireReanim->SetPosition(mPosX + aOffsetX, mPosY + aOffsetY);
		aFireReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
		aFireReanim->mAnimRate = RandRangeFloat(50.0f, 80.0f);
		AttachReanim(mAttachmentID, aFireReanim, aOffsetX, aOffsetY);
	}

	aFireReanim->mColorOverride = Color(0, 0, 0, 255);
	aFireReanim->mFilterEffect = FilterEffect::FILTER_EFFECT_NONE;
	aFireReanim->mExtraOverlayColor = Color(10, 10, 10, 150);
}

void Projectile::ConvertToPea(int theGridX)
{
	if (mHitTorchwoodGridX == theGridX)
		return;

	AttachmentDie(mAttachmentID);
	mProjectileType = ProjectileType::PROJECTILE_PEA;
	mHitTorchwoodGridX = theGridX;
	mApp->PlayFoley(FoleyType::FOLEY_THROW);
}

ProjectileDefinition& Projectile::GetProjectileDef()
{
	ProjectileDefinition& aProjectileDef = gProjectileDefinition[(int)mProjectileType];
	TOD_ASSERT(aProjectileDef.mProjectileType == mProjectileType);

	return aProjectileDef;
}

bool Projectile::GetGargantuars(int theRow, int theX, int theY, int theRadius, int theRowRange)
{
	Zombie* aZombie = nullptr;
	while (mBoard->IterateZombies(aZombie))
	{
		if (!aZombie->IsDeadOrDying() && !aZombie->mMindControlled && (aZombie->mZombieType == ZombieType::ZOMBIE_GARGANTUAR || aZombie->mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR))
		{
			Rect aZombieRect = aZombie->GetZombieRect();
			int aRowDist = aZombie->mRow - theRow;

			if (aRowDist <= theRowRange && aRowDist >= -theRowRange && GetCircleRectOverlap(theX, theY, theRadius, aZombieRect))
				return true;
		}
	}
	return false;
}