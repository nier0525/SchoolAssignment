#pragma once

#include <iostream>
using namespace std;

#define CHARSIZE 128		// char �ڷ��� ũ��

#define MAXCOUNT 5			// ĳ���� �ִ� ���� ����
#define GAMEMONEY 10000		// ���� �⺻ ���ӸӴ�

#define NODATA 0			// NODATA ����Ʈ ��
#define LISTSIZE 10			// ����Ʈ ũ��

// �޴� ������
enum { NEWLOG = 1, LOGIN, EXIT };		// �α���
enum { NEW = 1, DEL, BUY, SELL, LOGOUT, DELLOG };	// ����

// ĳ���� ����, ������ ���� ������
enum { ELF = 1, OAK, HUMAN };
enum { PORTION = 1, ARMOR, WENPON };

// ������ ���� ���� ������
enum { HP = 1, MP };
enum { HEAD = 1, TOP, SHIELD };
enum { SWORD = 1, BOW, WAND };