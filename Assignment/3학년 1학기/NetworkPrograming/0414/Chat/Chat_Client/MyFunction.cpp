#include "MyFunction.h"

void Add(char Arr[][MSGSIZE], int* count, ElementType data) {
	if (*count > MAX_SIZE - 1) {
		Sort(Arr, *count);
		strcpy(Arr[*count - 1], data);
	}
	else {
		strcpy(Arr[*count], data);
		*count += 1;
	}
}

void Remove(char Arr[][MSGSIZE], int* count, ElementType data) {
	char temp[128];
	for (int i = 0; i < *count; i++) {
		if (!strcmp(Arr[i], data)) {
			for (int j = i; j < *count - 1; j++) {
				strcpy(temp, Arr[j]);
				strcpy(Arr[j], Arr[j + 1]);
				strcpy(Arr[j + 1], temp);
			}
			break;
		}
	}

	ZeroMemory(&Arr[*count], sizeof(MSGSIZE));
	*count -= 1;
}

void Sort(char Arr[][MSGSIZE], int count) {
	char temp[128];
	for (int i = 0; i < count - 1; i++) {
		strcpy(temp, Arr[i]);
		strcpy(Arr[i], Arr[i + 1]);
		strcpy(Arr[i + 1], temp);
	}
}

void InputChar(WPARAM wParam, char* text, int* len, int size) {
	*len = strlen(text);
	if (wParam == VK_BACK && *len == NULL) {
		return;
	}
	if (wParam == VK_BACK && *len != NULL) {
		text[*len - 1] = NULL;
	}
	if (*len <= size) {
		text[*len] = (char)wParam;
		text[*len + 1] = NULL;
	}
}

double Lengthpts(int x1, int y1, int x2, int y2) {
	return (sqrt((float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))));
}

bool CollisionEnterPoint(int x, int y, int mx, int my, int x_size, int y_size) {
	if ((x + x_size >= mx && mx >= x - x_size) && (y + y_size >= my && my >= y - y_size)) {
		return true;
	}
	else {
		return false;
	}
}

bool RoomView(SocketPakage* socket, ROOMLIST& room) {
	int retval;
	ZeroMemory(&socket->buf, sizeof(socket->buf));

	sprintf_s(socket->buf, "%d", (int)CALL);
	retval = sendto(socket->s_sock, socket->buf, strlen(socket->buf), 0,
		(SOCKADDR*)&socket->send_addr, sizeof(socket->send_addr));
	if (retval == SOCKET_ERROR) {
		err_display("send");
		return false;
	}

	ZeroMemory(&socket->buf, sizeof(socket->buf));
	socket->addrlen = sizeof(socket->peeraddr);
	retval = recvfrom(socket->s_sock, socket->buf, sizeof(socket->buf), 0,
		(SOCKADDR*)&socket->peeraddr, &socket->addrlen);
	if (retval == SOCKET_ERROR) {
		err_display("recv");
		return false;
	}

	if (memcmp(&socket->peeraddr, &socket->send_addr, sizeof(&socket->peeraddr))) {
		MessageBox(nullptr, "¼­¹ö¿Í ¿¬°áÀÌ ²÷°å½À´Ï´Ù.", "DisConnet", MB_OK);
		return false;
	}

	memcpy(&room, socket->buf, sizeof(ROOMLIST));
	return true;
}

bool RoomSelect(SocketPakage* sock, User_Info& user, int num) {
	int retval;
	ZeroMemory(&sock->buf, sizeof(sock->buf));
	sprintf_s(sock->buf, "%d", num);

	retval = sendto(sock->s_sock, sock->buf, strlen(sock->buf), 0,
		(SOCKADDR*)&sock->send_addr, sizeof(sock->send_addr));
	if (retval == SOCKET_ERROR) {
		err_display("send");
		return false;
	}

	if (num == NULL) {
		return false;
	}

	ZeroMemory(&sock->buf, sizeof(sock->buf));
	sock->addrlen = sizeof(sock->peeraddr);
	retval = recvfrom(sock->s_sock, sock->buf, sizeof(sock->buf), 0,
		(SOCKADDR*)&sock->peeraddr, &sock->addrlen);
	if (retval == SOCKET_ERROR) {
		err_display("recv");
		return false;
	}

	if (memcmp(&sock->peeraddr, &sock->send_addr, sizeof(&sock->peeraddr))) {
		MessageBox(nullptr, "¼­¹ö¿Í ¿¬°áÀÌ ²÷°å½À´Ï´Ù.", "DisConnet", MB_OK);
		return false;
	}

	sock->buf[retval] = '\0';
	strcpy(user.room_ip, sock->buf);
	return true;
}

bool JoinMessage(SocketPakage* sock, User_Info user) {
	int retval;

	ZeroMemory(&sock->buf, sizeof(sock->buf));
	if (user.state) {
		sprintf_s(user.msg, "[ %s ´ÔÀÌ ÀÔÀåÇß½À´Ï´Ù ]", user.name);
	}
	else {
		sprintf_s(user.msg, "[ %s ´ÔÀÌ ÅðÀåÇß½À´Ï´Ù ]", user.name);
	}

	memcpy(sock->buf, &user, sizeof(User_Info));
	retval = sendto(sock->s_sock, sock->buf, sizeof(sock->buf), 0,
		(SOCKADDR*)&sock->send_addr, sizeof(sock->send_addr));
	if (retval == SOCKET_ERROR) {
		err_display("send");
		return false;
	}
	return true;
}

bool SendToMessage(SocketPakage* sock, User_Info& user) {
	int retval;

	ZeroMemory(&sock->buf, sizeof(BUFSIZE));
	memcpy(sock->buf, &user, sizeof(User_Info));

	retval = sendto(sock->s_sock, sock->buf, sizeof(sock->buf), 0, (SOCKADDR*)&sock->send_addr, sizeof(sock->send_addr));
	if (retval == SOCKET_ERROR) {
		err_display("send");
		return false;
	}

	ZeroMemory(&user.chat, sizeof(user.chat));
	return true;
}

DWORD WINAPI RecvThread(LPVOID arg) {
	int retval;
	SocketPakage* sock = (SocketPakage*)arg;
	
	User_Info user;
	char msg[MSGSIZE];

	while (1) {
		ZeroMemory(&sock->buf, sizeof(sock->buf));
		sock->addrlen = sizeof(sock->peeraddr);
		retval = recvfrom(sock->r_sock, sock->buf, sizeof(sock->buf), 0,
			(SOCKADDR*)&sock->peeraddr, &sock->addrlen);
		if (retval == SOCKET_ERROR) {
			err_display("recv");
			break;
		}

		memcpy(&user, sock->buf, sizeof(User_Info));
		if (user.chat[0] != NULL) {
			sprintf_s(msg, "[%d] %s ´Ô : %s", ntohs(sock->peeraddr.sin_port), user.name, user.chat);
			Add(sock->TextArray, &sock->current, msg);
		}
		else {
			Add(sock->TextArray, &sock->current, user.msg);

			//if (user.state)
			//	Add(sock->UserArray, &sock->count, user.name);
			//else
			//	Remove(sock->UserArray, &sock->count, user.name);
		}
	}
	return 0;
}

void LoginDisplay(HWND hWnd, HDC hdc, User_Info user) {
	char Text[128];

	Rectangle(hdc, 400 - 150, 300 - 70, 400 + 150, 300 + 70);
	Rectangle(hdc, 400 - 120, 280 - 15, 400 + 120, 280 + 15);
	TextOut(hdc, 290, 270, user.name, strlen(user.name));
	strcpy(Text, "Name");
	TextOut(hdc, 380, 240, Text, strlen(Text));

	Rectangle(hdc, 400 - 50, 340 - 20, 400 + 50, 340 + 20);
	strcpy(Text, "Enter or Click");
	TextOut(hdc, 357, 330, Text, strlen(Text));
}

void RoomSelectDisPlay(HWND hWnd, HDC hdc, User_Info user, ROOMLIST room) {
	char Text[128];

	for (int i = 0; i < room.roomsize; i++) {
		Rectangle(hdc, 400 - 200, (70 + (70 * i)) - 30, 400 + 200, (70 + (70 * i)) + 30);
		TextOut(hdc, 375, (60 + (70 * i)), room.roomname[i], strlen(room.roomname[i]));
	}

	// Back
	Rectangle(hdc, 400 - 50, 450 - 20, 400 + 50, 450 + 20);
	strcpy(Text, "Back");
	TextOut(hdc, 385, 440, Text, strlen(Text));
}

void ChattingDisPlay(HWND hWnd, HDC hdc, User_Info user, ROOMLIST room, int count, char chat[][MSGSIZE]) {
	char Text[128];

	// Info
	Rectangle(hdc, 400 - 100, 25 - 20, 400 + 100, 25 + 20);
	TextOut(hdc, 370, 17, room.roomname[user.room_num], strlen(room.roomname[user.room_num]));

	// input
	Rectangle(hdc, 290 - 250, 500 - 20, 290 + 250, 500 + 20);
	TextOut(hdc, 60, 490, user.chat, strlen(user.chat));

	// output
	Rectangle(hdc, 290 - 250, 250 - 200, 290 + 450, 250 + 200);
	for (int i = 0; i < count; i++) {
		TextOut(hdc, 60, (60 + (20 * i)), chat[i], strlen(chat[i]));
	}

	// Join User Count
	Rectangle(hdc, 700 - 40, 25 - 20, 700 + 40, 25 + 20);
	strcpy(Text, "Member");
	TextOut(hdc, 675, 15, Text, strlen(Text));

	// enter
	Rectangle(hdc, 600 - 40, 500 - 20, 600 + 40, 500 + 20);
	strcpy(Text, "Enter");
	TextOut(hdc, 585, 490, Text, strlen(Text));

	// exit
	Rectangle(hdc, 700 - 40, 500 - 20, 700 + 40, 500 + 20);
	strcpy(Text, "Back");
	TextOut(hdc, 685, 490, Text, strlen(Text));
}

void MemberDIsPlay(HWND hWnd, HDC hdc, int count, char list[][MSGSIZE]) {
	Rectangle(hdc, 640 - 100, 250 - 200, 640 + 100, 250 + 200);
	for (int i = 0; i < count; i++) {
		TextOut(hdc, 548, (60 + (20 * i)), list[i], strlen(list[i]));
	}
}