#pragma once
#include "Object3D.h"
#include "Texture.h"
#include "camera.h"
#include "FBXLoader.h"


/// <summary>
/// ���L�����̒e
/// </summary>
class PlayerBullet
{
public:
	PlayerBullet();
	~PlayerBullet();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	/// <param name="velocity">���x</param>
	void Initlize(const Vector3& position,const Vector3& velocity);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(const Camera& camera);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(AnimationModel* model);

	bool IsDead()const { return isDead_; }

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	Vector3 GetWorldPosition();

private:

	//�e�̖{��
	Object3D obj_;
	

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;

	

	//���x
	Vector3	Velocity_;

	//����<frm>
	static const int32_t kLifeTime = 60 * 100;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;

	//�f�X�t���O
	bool isDead_ = false;

};

