package Project1;

public class Elf extends Character {

	public Elf() {
		this.name = "None";
		this.brood = "����";
	}
	public Elf(String name)
	{
		this.name = name;
		this.brood = "����";
	}
	
	public void ActiveSkill()
	{
		System.out.println("�ӻ�");
	}
	
	public void PassiveSkill()
	{
		System.out.println("��ø�� ���");
	}
}
