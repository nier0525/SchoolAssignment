using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.AI;
using UnityEngine.SceneManagement;

public class Chase : MonoBehaviour {

    public Transform Target;
    Animator anim;
    NavMeshAgent agent;

    public int TrackingDistance = 1000;
    public int AttackRange = 3;

    public float WalkingSpeed = 0.05f;
    public float AttackDamage = 5.0f;

    bool target_alive;
    bool done = false;

    private float dist;

    private string[] tags = { "Tower", "Player" };
    GameObject[] taggedWall = { };

    // Use this for initialization
    void Start()
    {
        anim = GetComponent<Animator>();
        anim.SetBool("isWalking", false);
        anim.SetBool("isIdle", false);
        anim.SetBool("isAttacking", false);

        print("Target : " + Target);
        agent = GetComponent<NavMeshAgent>();
    }

    void FindNextTarget()
    {
        Debug.Log("Finding Next Target . . .");

        float closestDistSpr = Mathf.Infinity;
        Transform closestWall = null;

        foreach (string tag in tags)
        {
            GameObject[] taggedWalls = GameObject.FindGameObjectsWithTag(tag);
            foreach(GameObject taggedWall in taggedWalls)
            {
                Vector3 objpos = taggedWall.transform.position;
                dist = (objpos - transform.position).sqrMagnitude;
                if (dist < closestDistSpr)
                {
                    closestDistSpr = dist;
                    closestWall = taggedWall.transform;
                }
            }
            Target = closestWall;
        }

        if (Target == null)
        {
            SceneManager.LoadScene("GameLoseScene");
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (Target != null)
        {
            target_alive = Target.GetComponent<HP_Status>().alive;
            agent.SetDestination(Target.position);

            // if distance from player is within 50 units away from player
            if (target_alive && (Vector3.Distance(Target.position, this.transform.position) < TrackingDistance))
            {
                Vector3 direction = Target.position - this.transform.position;
                direction.y = 0;    // avoid tipping backwards when too close !
                this.transform.rotation = Quaternion.Slerp(this.transform.rotation, Quaternion.LookRotation(direction), 0.1f);

                // since we are in tracking distance
                anim.SetBool("isWalking", true);

                // keep walking if not close enough
                if (direction.magnitude > AttackRange)
                {
                    anim.SetBool("isIdle", false);
                    anim.SetBool("isWalking", true);
                    this.transform.Translate(0, 0, WalkingSpeed);
                    anim.SetBool("isAttacking", false);
                }

                // Attack
                else
                {
                    anim.SetBool("isWalking", false);
                    anim.SetBool("isAttacking", true);
                }
            }

            // too far away, simply idle
            else
            {
                anim.SetBool("isIdle", true);
                anim.SetBool("isWalking", false);
                anim.SetBool("isAttacking", false);
            }
        }
        else
        {
            anim.SetBool("isIdle", true);
            anim.SetBool("isWalking", false);
            anim.SetBool("isAttacking", false);

            FindNextTarget();
        }
    }
}
