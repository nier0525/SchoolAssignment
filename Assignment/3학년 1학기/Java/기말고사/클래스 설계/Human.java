package Project1;

public class Human extends Character {
	
	public Human() {
		this.name = "None";
		this.brood = "�ΰ�";
	}
	public Human(String name) { 
		this.name = name;
		this.brood = "�ΰ�";
	}
	
	public void ActiveSkill()
	{
		System.out.println("����");
	}
	
	public void PassiveSkill()
	{
		System.out.println("���� ���");
	}
}
