using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class DiabloShadow : DiabloShadowMotion
{
    DiabloControl DControl;
    Animator Anim;
    GameObject Player;
    Vector2 PlayerPos;
    public float Angle;
    public Color Original_color;
    float distance;

    bool startInv = false;

    private void Start()
    {
        Original_color = gameObject.GetComponent<SpriteRenderer>().color;
        Anim = GetComponent<Animator>();
        Player = GameObject.Find("Sorceress");
        DControl = GetComponentInParent<DiabloControl>();
    }

    private void Update()
    {
        if (DControl.death)
            GetComponent<SpriteRenderer>().color = new Color(0, 0, 0, 0);

        if (!GetComponentInParent<PathFinding>().isMove)
            Angle = GetComponentInParent<DiabloControl>().Angle;
        else
            Angle = GetComponentInParent<PathFinding>().Angle;

        PlayerPos = Player.transform.position;
        distance = (Player.transform.position - this.transform.position).magnitude;

        if(DControl.s_Walk)
            Walk(Anim, PlayerPos, Angle);
        else
            Anim.SetBool("Walk", false);

        if (DControl.s_Run)
            Run(Anim, PlayerPos, Angle);
        else
            Anim.SetBool("Run", false);

        if(DControl.s_Attack)
            Attack(Anim, PlayerPos, Angle);
        else
            Anim.SetBool("Attack", false);

        if (DControl.s_RunAndAttack)
            RunAndAttack(Anim, PlayerPos, Angle);
        else
            Anim.SetBool("RunAttack", false);

        if (DControl.s_UseLazer)
            UseLazer(Anim, PlayerPos, Angle);
        else
            Anim.SetBool("UseLazer", false);

        if (DControl.s_UseFire)
            UseFire(Anim, PlayerPos, Angle);
        else
            Anim.SetBool("UseFire", false);

        if (DControl.s_UseCold)
            UseCold(Anim, PlayerPos, Angle);
        else
            Anim.SetBool("UseCold", false);

        if (DControl.s_UseInv)
        {
            if (!startInv)
            {
                startInv = true;
                StartCoroutine(InvStart());
            }
        }
        
        
    }

    IEnumerator InvStart()
    {
        SpriteRenderer sr = GetComponent<SpriteRenderer>();
        Color color = sr.color;

        Anim.SetBool("UseInv", true);

        for (float i = 1f; i >= 0; i -= 0.05f)
        {
            color.a = i;
            sr.color = color;
            yield return new WaitForSeconds(0.1f);
        }
        color.a = 0f;
        sr.color = color;

        yield return new WaitForSeconds(0.5f);
        Anim.SetBool("UseInv", false);

        yield return new WaitForSeconds(1.5f);
        startInv = false;
    }

    //IEnumerator InvisibleAttack()
    //{
    //    Anim.SetBool("Run", false);

    //    Debug.Log("체크1");
    //    SpriteRenderer sr = GetComponent<SpriteRenderer>();
    //    Color color = sr.color;
    //    Debug.Log("체크2");
    //    while (DControl.InvAttackCount <= 5)
    //    {
    //        Debug.Log("체크3");

    //        Debug.Log(DControl.AttackStart);
    //        if (DControl.AttackStart)
    //        {
    //            Debug.Log("체크4");
    //            for (float i = 0f; i <= 0.7f; i += 0.1f)
    //            {
    //                color.a = i;
    //                sr.color = color;
    //                yield return new WaitForSeconds(0.01f);
    //            }

    //            yield return new WaitForSeconds(0.5f);

    //            for (float i = 0.7f; i >= 0; i -= 0.1f)
    //            {
    //                color.a = i;
    //                sr.color = color;
    //                yield return new WaitForSeconds(0.01f);
    //            }

    //            yield return new WaitForSeconds(0.7f);
    //        }
    //    }

    //    if (DControl.InvAttackCount >= 5)
    //    {
    //        for (float i = 0f; i <= 0.7f; i += 0.1f)
    //        {
    //            color.a = i;
    //            sr.color = color;
    //            yield return new WaitForSeconds(0.01f);
    //        }

    //        yield return new WaitForSeconds(1.5f);

    //        startInv = false;
    //    }
    //}
}
