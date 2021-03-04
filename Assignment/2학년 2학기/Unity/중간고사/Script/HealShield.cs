using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HealShield : MonoBehaviour
{
    public GameObject Boss;

    int maxlife;

    // Start is called before the first frame update
    void Start()
    {
        if (Boss.gameObject.name == "Boss_Chaos")
        {
            maxlife = 8500;
        }

        if (Boss.gameObject.name == "Boss_KimeraDragon")
        {
            maxlife = 13000;
        }

        Debug.Log(maxlife);
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Bullet")
        {
            Destroy(collision.gameObject);

            if (Boss.gameObject.name == "Boss_Chaos")
            {
                if (Boss.GetComponent<Boss_Chaos>().life < maxlife)
                {
                    Boss.GetComponent<Boss_Chaos>().life += FireShot.Attack;
                }
            }

            if (Boss.gameObject.name == "Boss_KimeraDragon")
            {
                if (Boss.GetComponent<Boss_KimeraDragon>().life < maxlife) ;
                {
                    Boss.GetComponent<Boss_KimeraDragon>().life += FireShot.Attack;
                }
            }
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
