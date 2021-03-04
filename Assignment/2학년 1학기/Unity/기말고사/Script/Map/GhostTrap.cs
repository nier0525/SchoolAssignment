using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GhostTrap : MonoBehaviour
{
    public bool GhostTrapOn;
    public GameObject Right;
    public GameObject Demon, Road, Chaos;
    public GameObject Zone01, Zone02;
    public AudioSource On;

    Animator ani;
    Color temp;

    // Start is called before the first frame update
    void Start()
    {
        GhostTrapOn = false;
        ani = gameObject.GetComponent<Animator>();
        temp = gameObject.GetComponent<SpriteRenderer>().color;
    }

    private void OnMouseOver()
    {
        gameObject.GetComponent<SpriteRenderer>().color = new Color(1, 1, 1, 1);
    }

    private void OnMouseExit()
    {
        gameObject.GetComponent<SpriteRenderer>().color = temp;
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Player")
        {
            if (!GhostTrapOn)
            {
                On.Play();
                for (int i = 0; i < 5; i++)
                {
                    GameObject demon = Instantiate(Demon) as GameObject;
                    demon.transform.position = Zone01.transform.position;
                    demon.GetComponent<DemonStatus>().hp *= 5;
                    demon.GetComponent<DemonStatus>().Att *= 2;
                    demon.GetComponent<DemonStatus>().Def *= 2;
                    demon.GetComponent<DemonStatus>().exp *= 3;

                    DemonAI.range = 6f;
                }
                for (int i = 0; i < 5; i++)
                {
                    GameObject chaos = Instantiate(Chaos) as GameObject;
                    chaos.transform.position = Zone01.transform.position;
                    chaos.GetComponent<ChaosStatus>().hp *= 5;
                    chaos.GetComponent<ChaosStatus>().Att *= 2;
                    chaos.GetComponent<ChaosStatus>().Def *= 2;
                    chaos.GetComponent<ChaosStatus>().exp *= 3;

                    ChaosAI.range = 6f;
                }
                for (int i = 0; i < 5; i++)
                {
                    GameObject road = Instantiate(Road) as GameObject;
                    road.transform.position = Zone01.transform.position;
                    road.GetComponent<SkullStatus>().hp *= 5;
                    road.GetComponent<SkullStatus>().Att *= 2;
                    road.GetComponent<SkullStatus>().Def *= 2;
                    road.GetComponent<SkullStatus>().exp *= 3;

                    SkullAI.range = 6f;
                }

                for (int i = 0; i < 5; i++)
                {
                    GameObject demon = Instantiate(Demon) as GameObject;
                    demon.transform.position = Zone02.transform.position;
                    demon.GetComponent<DemonStatus>().hp *= 5;
                    demon.GetComponent<DemonStatus>().Att *= 2;
                    demon.GetComponent<DemonStatus>().Def *= 2;
                    demon.GetComponent<DemonStatus>().exp *= 3;

                    DemonAI.range = 6f;
                }
                for (int i = 0; i < 5; i++)
                {
                    GameObject chaos = Instantiate(Chaos) as GameObject;
                    chaos.transform.position = Zone02.transform.position;
                    chaos.GetComponent<ChaosStatus>().hp *= 5;
                    chaos.GetComponent<ChaosStatus>().Att *= 2;
                    chaos.GetComponent<ChaosStatus>().Def *= 2;
                    chaos.GetComponent<ChaosStatus>().exp *= 3;

                    ChaosAI.range = 6f;
                }
                for (int i = 0; i < 5; i++)
                {
                    GameObject road = Instantiate(Road) as GameObject;
                    road.transform.position = Zone02.transform.position;
                    road.GetComponent<SkullStatus>().hp *= 5;
                    road.GetComponent<SkullStatus>().Att *= 2;
                    road.GetComponent<SkullStatus>().Def *= 2;
                    road.GetComponent<SkullStatus>().exp *= 3;

                    SkullAI.range = 6f;
                }
            }

            GhostTrapOn = true;
            ani.SetTrigger("On");
            ani.SetTrigger("Stay");
            Right.GetComponent<Animator>().SetTrigger("On");
            Right.GetComponent<Animator>().SetTrigger("Stay");

        }
    }

    // Update is called once per frame
    void Update()
    {

    }
}
