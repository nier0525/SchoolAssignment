using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DiabloShadowMotion : MonoBehaviour
{
    //걷기
    protected void Walk(Animator Anim, Vector2 playerPos, float Angle)
    {
        Anim.SetFloat("Angle", Angle);
        Anim.SetBool("Run", false);
        Anim.SetBool("Walk", true);
    }

    //걷기 후 공격
    protected void Attack(Animator Anim, Vector2 playerPos, float Angle)
    {
        Anim.SetFloat("Angle", Angle);
        Anim.SetBool("Attack", true);
    }

    //달리기
    protected void Run(Animator Anim, Vector2 playerPos, float Angle)
    {
        Anim.SetFloat("Angle", Angle);
        Anim.SetBool("Walk", false);
        Anim.SetBool("Run", true);
    }

    //달린 후 공격
    protected void RunAndAttack(Animator Anim, Vector2 playerPos, float Angle)
    {
        Anim.SetFloat("Angle", Angle);
        Anim.SetBool("RunAttack", true);
    }

    //레이저 스킬
    protected void UseLazer(Animator Anim, Vector2 playerPos, float Angle)
    {
        Anim.SetFloat("Angle", Angle);
        Anim.SetBool("UseLazer", true);

    }

    //파이어 스킬
    protected void UseFire(Animator Anim, Vector2 playerPos, float Angle)
    {
        Anim.SetFloat("Angle", Angle);
        Anim.SetBool("UseFire", true);
    }

    //콜드 스킬
    protected void UseCold(Animator Anim, Vector2 playerPos, float Angle)
    {
        Anim.SetFloat("Angle", Angle);
        Anim.SetBool("UseCold", true);
    }

    protected float GetDegree(Vector2 playerPos, Vector2 mousePos)
    {
        float angle = Mathf.Atan2(mousePos.y - playerPos.y, mousePos.x - playerPos.x) * 180 / Mathf.PI;
        if (angle < 0) angle += 360;

        return angle;
    }
}
