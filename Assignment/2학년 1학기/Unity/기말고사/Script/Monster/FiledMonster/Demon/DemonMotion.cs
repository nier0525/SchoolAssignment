using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DemonMotion : MonoBehaviour
{
    public Animator ani;
    public float work_speed;
    Vector3 mousepos;

    public float Angle = 135f;
    public bool isWork;
    public bool isAtt;
    public bool isSkill;
    PathFinding path;

    // Start is called before the first frame update
    void Start()
    {
        path = GetComponent<PathFinding>();
        work_speed = 1f;
        ani = gameObject.GetComponent<Animator>();
        ani.SetFloat("Angle", Angle);
        isWork = false;
        isAtt = false;

    }

    // Update is called once per frame
    void FixedUpdate()
    {
        ani.SetFloat("Angle", Angle);

        if (isWork && !isAtt && !path.isMove)
        {
            transform.position = Vector2.MoveTowards(transform.position, mousepos, work_speed * Time.deltaTime);
            float dis = Vector2.Distance(transform.position, mousepos);
            if (dis < 0.1f)
            {
                ani.SetBool("Work", false);
                isWork = false;
            }
        }

        if (!isAtt)
        {
            ani.SetBool("Attak", false);
        }
    }


    public void Work(Vector3 pos)
    {
        Vector2 dir = (Vector2)pos - (Vector2)transform.position;
        Angle = Mathf.Atan2(dir.x, dir.y) * Mathf.Rad2Deg;

        ani.SetFloat("Angle", Angle);
        ani.SetBool("Work", true);
        mousepos = pos;
        isWork = true;
    }

    public void Attak(Vector3 pos)
    {
        Vector2 dir = (Vector2)pos - (Vector2)transform.position;
        Angle = Mathf.Atan2(dir.x, dir.y) * Mathf.Rad2Deg;

        ani.SetFloat("Angle", Angle);
        ani.SetBool("Attak", true);
        isWork = false;
        isAtt = true;
    }

    public void Skill(Vector3 pos)
    {
        ani.SetFloat("Angle", Angle);
        ani.SetBool("Skill", true);
        isWork = false;
        isSkill = true;
    }
}
