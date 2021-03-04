package Project1;

import java.util.Scanner;

public class MainProgram {

	public static Character CharacterSelect()
	{
		Character player = null;
		Scanner scan = new Scanner(System.in);
		
		while (true)
		{
		System.out.println("1. 인간 / 2. 엘프 / 3. 오크");
		System.out.print("선택 : ");
		
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
			System.out.println("잘못된 입력입니다.");
			continue;
		}
		break;
		}
		
		System.out.println();
		System.out.print("캐릭터 이름 : ");
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
			System.out.print("액티브 스킬 : ");
			player.ActiveSkill();
			System.out.print("패시브 스킬 : ");
			player.PassiveSkill();
		}	
	}

}
