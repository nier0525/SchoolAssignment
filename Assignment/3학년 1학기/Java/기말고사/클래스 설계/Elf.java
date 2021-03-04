package Project1;

public class Elf extends Character {

	public Elf() {
		this.name = "None";
		this.brood = "¿¤ÇÁ";
	}
	public Elf(String name)
	{
		this.name = name;
		this.brood = "¿¤ÇÁ";
	}
	
	public void ActiveSkill()
	{
		System.out.println("¼Ó»ç");
	}
	
	public void PassiveSkill()
	{
		System.out.println("¹ÎÃ¸¼º »ó½Â");
	}
}
