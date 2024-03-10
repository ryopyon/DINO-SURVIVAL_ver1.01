//=============================================================================
//
// ���͏��� [input.h]
// Author : HAL���� ������
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �v���O������������Ƃ��Ɏg��
#define	USE_KEYBOARD				// �錾����ƃL�[�{�[�h�ő���\�ɂȂ�
#define	USE_MOUSE					// �錾����ƃ}�E�X�ő���\�ɂȂ�
#define	USE_PAD						// �錾����ƃp�b�h�ő���\�ɂȂ�


/* game pad��� */
#define STICK_L_UP		0x00000001l	// ���X�e�B�b�N�L�[��(.IY<0)
#define STICK_L_DOWN	0x00000002l	// ���X�e�B�b�N�L�[��(.IY>0)
#define STICK_L_LEFT	0x00000004l	// ���X�e�B�b�N�L�[��(.IX<0)
#define STICK_L_RIGHT	0x00000008l	// ���X�e�B�b�N�L�[�E(.IX>0)

#define STICK_R_UP		0x00010000l	// �E�X�e�B�b�N�L�[��(.IY<0)
#define STICK_R_DOWN	0x00020000l	// �E�X�e�B�b�N�L�[��(.IY>0)
#define STICK_R_LEFT	0x00040000l	// �E�X�e�B�b�N�L�[��(.IX<0)
#define STICK_R_RIGHT	0x00080000l	// �E�X�e�B�b�N�L�[�E(.IX>0)

#define BUTTON_UP		0x00100000l	// �\���L�[��(.IY<0)
#define BUTTON_DOWN		0x00200000l	// �\���L�[��(.IY>0)
#define BUTTON_LEFT		0x00400000l	// �\���L�[��(.IX<0)
#define BUTTON_RIGHT	0x00800000l	// �\���L�[�E(.IX>0)

#define BUTTON_X		0x00000010l	// �w�{�^��(.rgbButtons[0]&0x80)
#define BUTTON_A		0x00000020l	// �`�{�^��(.rgbButtons[1]&0x80)
#define BUTTON_B		0x00000040l	// �a�{�^��(.rgbButtons[2]&0x80)
#define BUTTON_Y		0x00000080l	// �x�{�^��(.rgbButtons[3]&0x80)
#define BUTTON_L		0x00000100l	// �k�{�^��(.rgbButtons[4]&0x80)
#define BUTTON_R		0x00000200l	// �q�{�^��(.rgbButtons[5]&0x80)
#define BUTTON_L2		0x00000400l	// �k�{�^��(.rgbButtons[6]&0x80)
#define BUTTON_R2		0x00000800l	// �q�{�^��(.rgbButtons[7]&0x80)
#define BUTTON_SELECT	0x00001000l	// �y�{�^��(.rgbButtons[8]&0x80)
#define BUTTON_START	0x00002000l	// �r�s�`�q�s�{�^��(.rgbButtons[9]&0x80)
#define BUTTON_LS		0x00004000l	// �y�{�^��(.rgbButtons[10]&0x80)
#define BUTTON_RS		0x00008000l	// �y�{�^��(.rgbButtons[11]&0x80)

#define GAMEPADMAX		4			// �����ɐڑ�����W���C�p�b�h�̍ő吔���Z�b�g

enum {	// �{�^���ɑ΂���rgbButtons[?]�z��̔z��ԍ�(DirectInput��ł�XYAB�{�^���ʒu�ɍ��킹��)
	rgbButtons_X,
	rgbButtons_A,
	rgbButtons_B,
	rgbButtons_Y,
	rgbButtons_L,
	rgbButtons_R,
	rgbButtons_L2,
	rgbButtons_R2,
	rgbButtons_SELECT,
	rgbButtons_START,
};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

//---------------------------- keyboard
BOOL GetKeyboardPress(int nKey);
BOOL GetKeyboardTrigger(int nKey);
BOOL GetKeyboardRepeat(int nKey);
BOOL GetKeyboardRelease(int nKey);

//---------------------------- mouse
BOOL IsMouseLeftPressed(void);      // ���N���b�N�������
BOOL IsMouseLeftTriggered(void);    // ���N���b�N�����u��
BOOL IsMouseRightPressed(void);     // �E�N���b�N�������
BOOL IsMouseRightTriggered(void);   // �E�N���b�N�����u��
BOOL IsMouseCenterPressed(void);    // ���N���b�N�������
BOOL IsMouseCenterTriggered(void);  // ���N���b�N�����u��
long GetMouseX(void);               // �}�E�X��X�����ɓ��������Βl
long GetMouseY(void);               // �}�E�X��Y�����ɓ��������Βl
long GetMouseZ(void);               // �}�E�X�z�C�[�������������Βl

//---------------------------- game pad
BOOL IsButtonPressed(int padNo,DWORD button);
BOOL IsButtonTriggered(int padNo,DWORD button);
BOOL IsButtonRelease(int padNo, DWORD button);

DIJOYSTATE2 GetDeviceState(int num = 0);

