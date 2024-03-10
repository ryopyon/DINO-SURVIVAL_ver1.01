//=============================================================================
//
// �L�[�t���[���t�@�C���ǂݍ��ݏ��� [keyframe.cpp]
// Author : HAL���� ������
//
//=============================================================================
#include "main.h"
#include "keyframe.h"
#include "player.h"
#include "enemy.h"
#include "boss.h"



//=============================================================================
// �A�j���[�V�����f�[�^�̏����� ���e�L�X�g
//=============================================================================
void LoadAnimDataText(char* FileName, std::vector<INTERPOLATION_DATA>& tbl)
{
	FILE* file;
	char line[256];
	float tx, ty, tz, rx, ry, rz, sx, sy, sz, frame;

	// �t�@�C�����J��
	file = fopen(FileName, "r");
	if (file == NULL) {
		perror("�t�@�C���̃I�[�v���Ɏ��s���܂���");
		return;
	}

	// �e�[�u�����J���ɂ���
	tbl.clear();

	// �t�@�C����ǂݍ���ŏ���������
	while (fgets(line, sizeof(line), file) != NULL)
	{
		(void)sscanf(line, "tx:%f ty:%f tz:%f rx:%f ry:%f rz:%f sx:%f sy:%f sz:%f frame:%f", &tx, &ty, &tz, &rx, &ry, &rz, &sx, &sy, &sz, &frame);

		tbl.push_back(
			{
			XMFLOAT3(tx * 10.0f, ty * 10.0f, tz * 10.0f),
			XMFLOAT3(XMConvertToRadians(rx), XMConvertToRadians(ry), XMConvertToRadians(rz)),
			XMFLOAT3(sx, sy, sz),
			frame
			}
		);
	}

	// �t�@�C�������
	fclose(file);
}


//=============================================================================
// �A�j���[�V�����f�[�^�̏����� ��CSV
//=============================================================================
void LoadAnimDataCSV(char* FileName, std::vector<INTERPOLATION_DATA>& tbl)
{
	FILE* file;
	char line[256];
	float tx, ty, tz, rx, ry, rz, sx, sy, sz, frame;

	// �t�@�C�����J��
	file = fopen(FileName, "r");
	if (file == NULL) {
		perror("�t�@�C���̃I�[�v���Ɏ��s���܂���");
		return;
	}

	// �e�[�u�����J���ɂ���
	tbl.clear();

	// �t�@�C����ǂݍ���ŏ���������
	fgets(line, sizeof(line), file);

	while (fgets(line, sizeof(line), file) != NULL)
	{
		(void)sscanf(line, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f", &tx, &ty, &tz, &rx, &ry, &rz, &sx, &sy, &sz, &frame);

		tbl.push_back(
			{
			XMFLOAT3(tx * 10.0f, ty * 10.0f, tz * 10.0f),
			XMFLOAT3(XMConvertToRadians(rx), XMConvertToRadians(ry), XMConvertToRadians(rz)),
			XMFLOAT3(sx, sy, sz),
			frame
			}
		);
	}

	// �t�@�C�������
	fclose(file);
}