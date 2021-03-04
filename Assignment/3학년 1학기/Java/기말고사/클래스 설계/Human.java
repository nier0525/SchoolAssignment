package Project1;

public class Human extends Character {
	
	public Human() {
		this.name = "None";
		this.brood = "인간";
	}
	public Human(String name) { 
		this.name = name;
		this.brood = "인간";
	}
	
	public void ActiveSkill()
	{
		System.out.println("협공");
	}
	
	public void PassiveSkill()
	{
		System.out.println("지력 상승");
	}
}
