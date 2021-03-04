using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HunterInstance : MonoBehaviour
{
    public GameObject[] Zone_2, Zone_3, Zone_4;
    public GameObject Hunter;

    // Start is called before the first frame update
    void Start()
    {
        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 2; j++)
            {
                {
                    GameObject hunter = Instantiate(Hunter) as GameObject;
                    hunter.transform.position = Zone_2[i].transform.position;
                }
            }

        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 3; j++)
            {
                {
                    GameObject hunter = Instantiate(Hunter) as GameObject;
                    hunter.transform.position = Zone_3[i].transform.position;
                }
            }

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 4; j++)
            {
                {
                    GameObject hunter = Instantiate(Hunter) as GameObject;
                    hunter.transform.position = Zone_4[i].transform.position;
                }
            }
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
