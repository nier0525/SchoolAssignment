using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MonsterTest : MonoBehaviour
{
    public GameObject Demon;
    //public GameObject DemonShadow_s;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.R))
        {
            GameObject Monster = Instantiate(Demon) as GameObject;            
            //GameObject Monster_s = Instantiate(DemonShadow_s) as GameObject;
            //Monster_s. transform.parent = Monster.transform;
            Monster.transform.position = new Vector2(0, 0);
        }
    }
}
