using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OgreShadowMotion : MonoBehaviour
{
    public GameObject demon;
    public Animator ani;
    IEnumerator Attack_c;
    int hp = 0;

    // Start is called before the first frame update
    void Start()
    {
        ani = GetComponent<Animator>();
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        ani.SetFloat("Angle", demon.GetComponent<OgreMotion>().Angle);

        if (!demon.GetComponent<OgreMotion>().isWork)
        {
            ani.SetBool("Work", false);
        }   

        if (demon.GetComponent<OgreStatus>().hp <= hp)
        {
            Dead();
            hp = -100;
        }
    }

    IEnumerator DeathWait()
    {
        yield return new WaitForSeconds(1.2f);
        Destroy(this.gameObject);
    }

    IEnumerator AttackWait()
    {
        if (!demon.GetComponent<OgreMotion>().isAtt)
        {
            yield return null;
        }
        else
        {
            ani.SetBool("Attak", true);
            yield return new WaitForSeconds(1f);
            ani.SetBool("Attak", false);
            yield return new WaitForSeconds(1f);
        }
    }

    public void Dead()
    {
        if (demon.GetComponent<OgreAI>().death)
        {
            ani.SetTrigger("Death");
            StartCoroutine(DeathWait());
        }
    }

    public void Work(Vector2 pos)
    {
       ani.SetBool("Work", true);
    }

    public void Attak(Vector2 pos)
    {
        ani.SetBool("Work", false);
        Attack_c = AttackWait();
        StartCoroutine(Attack_c);     
    }

    public void Hit(Vector2 pos)
    {
        ani.SetTrigger("Hit");
        ani.SetTrigger("HitEnd");
    }

    float GetAngle(Vector2 demonPos, Vector2 playerPos)
    {
        Vector2 dir = playerPos - demonPos;
        float angle = Mathf.Atan2(dir.x, dir.y) * Mathf.Rad2Deg;
        return angle;
    }
}
