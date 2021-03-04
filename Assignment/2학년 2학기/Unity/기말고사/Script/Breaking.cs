using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class Breaking : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {

    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.tag == "Monster_Body")
        {
            GameObject obj = other.gameObject.transform.root.gameObject;
            obj.GetComponent<NavMeshAgent>().acceleration = 0;
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
