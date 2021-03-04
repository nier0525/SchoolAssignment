using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

using UnityEngine.SceneManagement;

public class Monster_Move : MonoBehaviour
{
    public Transform Target;
    public AudioSource attack;
    Animator anim;

    NavMeshAgent agent;

    public string ani_idle;
    public string ani_attak;
    public string ani_work;
    public string ani_run;

    public int TrackingDistance = 1000;
    public int AttackRange = 3;

    public float WakingSpeed;
    public float AttackDamage;

    bool target_alive;
    bool done = false;

    float dist;

    string[] tags = { "Tower", "Player" };
    GameObject[] taggedWall = { };

    // Start is called before the first frame update
    void Start()
    {
        anim = GetComponent<Animator>();

        anim.SetBool(ani_work, false);
        anim.SetBool(ani_attak, false);
        anim.SetBool(ani_idle, false);
        anim.SetBool(ani_run, false);

        agent = GetComponent<NavMeshAgent>();
    }

    void FindNextTarget()
    {
        Debug.Log("Finding Next Target . . .");

        float closeDistSpr = Mathf.Infinity;
        Transform closeWall = null;

        foreach (string tag in tags)
        {
            GameObject[] taggedWalls = GameObject.FindGameObjectsWithTag(tag);

            foreach(GameObject taggedWall in taggedWalls)
            {
                Vector3 pos = taggedWall.transform.position;
                dist = (pos - transform.position).sqrMagnitude;

                if (dist < closeDistSpr)
                {
                    closeDistSpr = dist;
                    closeWall = taggedWall.transform;
                }
            }

            Target = closeWall;
        }
    }

    void Attack()
    {
        if (Target != null && (Target.position - transform.position).magnitude <= AttackRange + 0.5f)
        {
            if (attack != null)
                attack.Play();

            Target.GetComponent<HP_Status>().Hit_Damage(AttackDamage);
        }
    }

    void Attack_End()
    {
        anim.SetBool(ani_idle, true);
        done = false;
    }

    // Update is called once per frame
    void Update()
    {
        if (Target != null)
        {
            target_alive = Target.GetComponent<HP_Status>().alive;

            if (GetComponent<HP_Status>().hit)
            {
                
                Target = GameObject.Find("Player").transform;
            }

            if (target_alive && GetComponent<HP_Status>().alive
                && (Vector3.Distance(Target.position, this.transform.position) < TrackingDistance))
            {
                agent.SetDestination(Target.position);

                Vector3 direction = Target.position - this.transform.position;
                direction.y = 0;
                this.transform.rotation = Quaternion.Slerp(this.transform.rotation, Quaternion.LookRotation(direction), 0.1f);

                anim.SetBool(ani_work, true);

                if (direction.magnitude > AttackRange && !done)
                {
                    agent.avoidancePriority = 50;

                    anim.SetBool(ani_attak, false);
                    anim.SetBool(ani_idle, false);
                    anim.SetBool(ani_work, true);

                    this.transform.Translate(0, 0, WakingSpeed * Time.deltaTime);
                }

                else
                {
                    done = true;

                    agent.avoidancePriority = 49;

                    anim.SetBool(ani_work, false);
                    anim.SetBool(ani_attak, true);
                }
            }

            else
            {
                anim.SetBool(ani_idle, true);
                anim.SetBool(ani_work, false);
                anim.SetBool(ani_attak, false);
            }
        }

        else
        {
            anim.SetBool(ani_idle, true);
            anim.SetBool(ani_work, false);
            anim.SetBool(ani_attak, false);

            FindNextTarget();
        }
    }
}
