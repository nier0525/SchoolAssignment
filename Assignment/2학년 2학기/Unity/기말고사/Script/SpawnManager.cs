using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpawnManager : MonoBehaviour
{
    public GameObject[] ObjectToSpawn;
    public Transform[] pos;

    public float spawnDelay = 1.0f;
    public float countDown;
    public bool EnableSpawn = true;

    bool boss = false;
    bool done = false;

    // Start is called before the first frame update
    void Start()
    {
        countDown = spawnDelay / 2;
    }

    // Update is called once per frame
    void Update()
    {
        countDown -= Time.deltaTime;

        if (countDown < 0)
        {
            if (EnableSpawn)
            {
                for (int i = 0; i < 2; i++)
                    Instantiate(ObjectToSpawn[i], pos[Random.Range(0, 8)].position, transform.rotation);

                if (GameManager.Score > 700)
                    Instantiate(ObjectToSpawn[ObjectToSpawn.Length - 1], pos[Random.Range(0, 8)].position, transform.rotation);

                countDown = spawnDelay;
            }
        }
    }
}
