using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Spawner : MonoBehaviour {

	public GameObject wallPrefab;
	public float interval;

	public float range;

    public GameObject Monster, Monster_SpawnerPos;
    public float interval_monster;
    public float range_monster;

	// Use this for initialization
	void Start () {
        StartCoroutine(WallMove());

        if (PlayerPrefs.GetInt("NowStage") != 4)
        {
            StartCoroutine(MonsterMove());
        }
	}

	IEnumerator WallMove(){
		while(true){
			transform.position = new Vector3(transform.position.x, Random.Range(-range, range), transform.position.z);

			Instantiate(wallPrefab, transform.position, transform.rotation);

			yield return new WaitForSeconds(interval);
		}
	}

    IEnumerator MonsterMove()
    {
        while (true)
        {
            Monster_SpawnerPos.transform.position = new Vector3(Monster_SpawnerPos.transform.position.x
                , Random.Range(-range_monster, range_monster), transform.position.z);
            Instantiate(Monster, Monster_SpawnerPos.transform.position, Monster_SpawnerPos.transform.rotation);

            yield return new WaitForSeconds(interval_monster);
        }
    }

	// Update is called once per frame
	void Update () {
		
	}
}
