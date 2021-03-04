using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class IzualShadow : MonoBehaviour
{
    public GameObject demon;
    IzualShadowMotion shadow_m;
    Vector2 player_pos;

    int hp;

    // Start is called before the first frame update
    void Start()
    {
        shadow_m = GetComponent<IzualShadowMotion>();
        hp = demon.GetComponent<IzualStatus>().hp - 1;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        player_pos = GameObject.Find("Sorceress").transform.position;

        if (demon.GetComponent<IzualMotion>().isWork)
        {
            shadow_m.Work(player_pos);
        }

        if (demon.GetComponent<IzualMotion>().isSkill)
        {
            shadow_m.SKill(player_pos);
        }

        if (demon.GetComponent<IzualStatus>().hp < hp)
        {
            shadow_m.ani.SetTrigger("Hit");
            shadow_m.ani.SetTrigger("HitEnd");
            hp = demon.GetComponent<IzualStatus>().hp;
        }
    }
}
