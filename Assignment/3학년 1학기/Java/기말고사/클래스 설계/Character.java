package Project1;

abstract public class Character {
		protected String name;
		protected String brood;	
		
		Character() {
			this.name = "None";
			this.brood = "None";
		}
		Character(String name, String brood)
		{
			this.name = name;
			this.brood = brood;
		}
		
		public void SetName(String name) { 
			this.name = name;
		}
				
		public void PrintInfo()
		{
			System.out.println("캐릭터 이름 : " + name);
			System.out.println("캐릭터 종족 : " + brood);
		}
		
		abstract public void ActiveSkill();
		abstract public void PassiveSkill();
}
