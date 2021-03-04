using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DummyScript : MonoBehaviour
{

    public string ani;
    public bool Move;
    public float lengh;
    public float Speed;

    Vector3 pos;

    // Start is called before the first frame update
    void Start()
    {
        pos = transform.position;
    }

    public void Attack()
    {

    }

    public void Attack_End()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (Move)
        {
            GetComponent<Animator>().SetBool(ani, true);
            transform.Translate(0, 0, Speed * Time.deltaTime);

            if (lengh > 0)
            {
                if (transform.position.x > lengh)
                {
                    transform.position = pos;
                }
            }
            else
            {
                if (transform.position.x < lengh)
                {
                    transform.position = pos;
                }
            }
        }
    }
}
