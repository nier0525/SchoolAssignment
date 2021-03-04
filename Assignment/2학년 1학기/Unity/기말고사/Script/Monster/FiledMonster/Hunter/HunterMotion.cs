using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HunterMotion : MonoBehaviour
{
    public Animator ani;
    public float work_speed;
    Vector3 mousepos;
    PathFinding PfScript;

    public float Angle = 135f;
    public bool isWork;
    public bool isAtt;

    // Start is called before the first frame update
    void Start()
    {
        PfScript = GetComponent<PathFinding>();

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

        if (isWork && !isAtt && !PfScript.isMove)
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
}
