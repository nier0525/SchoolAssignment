using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Storm : MonoBehaviour
{
    public GameObject Bolt;

    // Start is called before the first frame update
    void Start()
    {
        StartCoroutine(StormActive());
    }

    IEnumerator StormActive()
    {
        for (int j = 0; j < 18; j++)
        {
            Instantiate(Bolt, transform.position, Quaternion.Euler(0, 0, -25 + (20 * j)));
            yield return new WaitForSeconds(0.2f);
        }
        Destroy(gameObject);
    }

    // Update is called once per frame
    void Update()
    {
        transform.Translate(-1.5f * Time.deltaTime, 0, 0);
    }
}
