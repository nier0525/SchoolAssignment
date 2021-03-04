package Project1;

import java.util.Scanner;

public class MainProgram {

	public static Character CharacterSelect()
	{
		Character player = null;
		Scanner scan = new Scanner(System.in);
		
		while (true)
		{
		System.out.println("1. �ΰ� / 2. ���� / 3. ��ũ");
		System.out.print("���� : ");
		
		int index = scan.nextInt();
		
		switch(index)
		{
		case 1:
			player = new Human();
			break;
			
		case 2:
			player = new Elf();
			break;
			
		case 3:
			player = new Oak();
			break;
			
		default:
			System.out.println("�߸��� �Է��Դϴ�.");
			continue;
		}
		break;
		}
		
		System.out.println();
		System.out.print("ĳ���� �̸� : ");
		String name = scan.next();
		
		player.SetName(name);
		System.out.println();
		
		return player;
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		var player = CharacterSelect();
				
		if (player != null)
		{
			player.PrintInfo();
			System.out.print("��Ƽ�� ��ų : ");
			player.ActiveSkill();
			System.out.print("�нú� ��ų : ");
			player.PassiveSkill();
		}	
	}

}
