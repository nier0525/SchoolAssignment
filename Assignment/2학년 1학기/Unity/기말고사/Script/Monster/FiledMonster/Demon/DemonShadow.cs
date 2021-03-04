using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DemonShadow : MonoBehaviour
{
    public GameObject demon;
    DemonShadowMotion shadow_m;
    Vector2 player_pos;

    int hp;

    // Start is called before the first frame update
    void Start()
    {
        shadow_m = GetComponent<DemonShadowMotion>();
        hp = demon.GetComponent<DemonStatus>().hp - 1;

    }

    // Update is called once per frame
    void FixedUpdate()
    {
        player_pos = GameObject.Find("Sorceress").transform.position;

        if (demon.GetComponent<DemonMotion>().isWork)
        {
            shadow_m.Work(player_pos);
        }

        if (demon.GetComponent<DemonStatus>().hp < hp)
        {
            shadow_m.ani.SetTrigger("Hit");
            shadow_m.ani.SetTrigger("HitEnd");
            hp = demon.GetComponent<DemonStatus>().hp;
        }
    }
}
