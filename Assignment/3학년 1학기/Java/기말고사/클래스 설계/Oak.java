package Project1;

public class Oak extends Character {

	public Oak() {
		this.name = "None";
		this.brood = "오크";
	}
	
	public Oak(String name)
	{
		this.name = name;
		this.brood = "오크";
	}
	
	public void ActiveSkill()
	{
		System.out.println("돌진");
	}
	
	public void PassiveSkill()
	{
		System.out.println("근력 상승");
	}

}
