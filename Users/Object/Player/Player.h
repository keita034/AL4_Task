#pragma once

#include "Input.h"
#include"ModelCreate.h"
#include"Transform.h"
#include"Material.h"
#include"GameCamera.h"
#include"SpriteCreat.h"
#include"Missile.h"
#include"WindowsApp.h"
#include"GameCollisionConflg.h"
#include"AudioManager.h"

class Enemy;

struct PlayerData
{
	std::shared_ptr<objModel> player;
	std::shared_ptr<objModel> missileModel;
	std::shared_ptr<GameCamera> camera;
	TextureData reticleTex;
	TextureData RockOnReticleTex;
	TextureData sceneChangeTex;
	TextureData silenPullUpTex;
	TextureData silenReturnTex;
	TextureData damageEffectTex[3];
	uint32_t missileSE = 0;
	uint32_t fighterSE = 0;
	uint32_t destroySE = 0;
	uint32_t gameOverScene = 0;
};

class Player : public OBB
{
private:

	//�N���X�ǂݍ���
	Input* input_ = nullptr;
	std::shared_ptr<objModel> player_;
	std::shared_ptr<objModel> missileModel_;
	std::unique_ptr<Transform> transform_{};
	std::shared_ptr<GameCamera> camera_;
	//�~�T�C��
	std::list<std::unique_ptr<Missile>> missiles_;
	std::unique_ptr<Sprite2D>reticle;
	TextureData reticleTex;
	Transform reticleTrans;

	std::unique_ptr<Sprite2D>RockOnReticle;
	TextureData RockOnReticleTex;
	Transform rockOnReticleTrans;

	std::unique_ptr<Sprite2D>sceneChange;
	TextureData sceneChangeTex_;
	Transform sceneChangeTransform;
	float alpha = 0.0f;
	float alphaTimer = 0.0f;
	bool animationFlag = false;
	char PADING[7]{};

	std::unique_ptr<Sprite2D>silenPullUp;
	TextureData silenPullUpTex_{};
	Transform silenPullUpTransform;

	std::unique_ptr<Sprite2D>silenReturn;
	TextureData silenReturnTex_{};
	Transform silenReturnTransform;

	std::unique_ptr<Sprite2D>damageEffect;
	TextureData damageEffectTex_[3];
	Transform damageEffectTransform;
	bool damageFlag = false;
	char PADING5[3]{};
	float damageAlpha = 0.5f;

	Enemy* enemy_ = nullptr;

	//�ϐ��錾
	float speed = 600.0f;
	char PADING4[4]{};
	
	//�~�T�C�����ˊԊu
	int32_t missileInterval = 0;
	int32_t bulletInterval = 0;
	char PADING2[4]{};
	//�錾
	AliceMathF::Vector3 targetVec_;

	float targetSize_ = 0.0f;

	AliceMathF::Vector3 velocity_;
	AliceMathF::Vector3 frontVec_;
	float vecDistance = 20.0f;
	AliceMathF::Vector3 oldTranslation_;

	bool RockOnFlag = false;
	char PADING3[3]{};
	int32_t attackInterval = 60 * 5;

	int32_t hp = 4;
	bool initflag = true;
	char PADING8[3]{};
	//�~�T�C�����ˉ�
	uint32_t missileSE = 0;
	//�e���ˉ�
	uint32_t bulletSE = 0;
	uint32_t fighterSE = 0;
	uint32_t destroySE = 0;
	char PADING6[4]{};
	Transform tmpTrasn;
	AliceMathF::Vector3 WorldPos;
	AliceMathF::Vector3 velocity2;
	AliceMathF::Vector3 tmpVec;

	char PADING7[4]{};
	std::list<std::shared_ptr<Enemy>> tmpEnemys;

	uint32_t gameOverScene = 0;

	float warningInterval = 0.0f;

public:
	Player();
	~Player();
	void Initialize(AliceMathF::Vector3* pos, PlayerData& data);
	void Complement(float& x1, float x2, float flame);//�⊮�֐�
	void Rotate();
	void MissileAttack();
	void Update(INT32& sceneNum);
	void Draw();
	void LockOn(std::list<std::shared_ptr<Enemy>>& enemys);
	bool LockOnFlag(Enemy* enemy);
	bool ReticleFlag();
	bool ReticleFlag(Enemy* enemy);
	virtual void OnCollision()override;
	void StopAudio();
	//�Q�b�^�[
	Transform* GetTransform();
	AliceMathF::Vector3 GetWorldPosition();
	std::list<std::unique_ptr<Missile>>& GetMissileList();
	int32_t GetHp();
private:
	//�R�s�[�R���X�g���N�^�E������Z�q�폜
	Player& operator=(const Player&) = delete;
	Player(const Player&) = delete;
	void RockOnReticleUpdate();

};

