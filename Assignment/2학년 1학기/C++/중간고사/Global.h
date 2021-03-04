#pragma once

#include <iostream>
using namespace std;

#define CHARSIZE 128		// char 자료형 크기

#define MAXCOUNT 5			// 캐릭터 최대 생성 개수
#define GAMEMONEY 10000		// 유저 기본 게임머니

#define NODATA 0			// NODATA 디폴트 값
#define LISTSIZE 10			// 리스트 크기

// 메뉴 열거형
enum { NEWLOG = 1, LOGIN, EXIT };		// 로그인
enum { NEW = 1, DEL, BUY, SELL, LOGOUT, DELLOG };	// 게임

// 캐릭터 종족, 아이템 종류 열거형
enum { ELF = 1, OAK, HUMAN };
enum { PORTION = 1, ARMOR, WENPON };

// 아이템 상제 종류 열거형
enum { HP = 1, MP };
enum { HEAD = 1, TOP, SHIELD };
enum { SWORD = 1, BOW, WAND };