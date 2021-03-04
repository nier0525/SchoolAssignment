using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class PlayerCtrl : MonoBehaviour
{
    private NavMeshAgent nv;
    public Transform targetTr;

    private Animator anim;
    public int hashRun = Animator.StringToHash("Run");
    public int hashFinish = Animator.StringToHash("isFinish");

    // Start is called before the first frame update
    void Start()
    {
        Application.targetFrameRate = 60;

        nv = GetComponent<NavMeshAgent>();
        anim = GetComponent<Animator>();

        // yield return new WaitForSeconds(3.0f);

        nv.SetDestination(targetTr.position);
        anim.SetTrigger(hashRun);
    }

    private void FixedUpdate()
    {
        nv.SetDestination(targetTr.position);

        if (transform.position.z < -43)
        {
            transform.position = new Vector3(0.6f, 3.6f, 43.7f);
        }
    }
}
