using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SkillShot : MonoBehaviour
{
    public float speed;
    static public int attck;

    // Start is called before the first frame update
    void Start()
    {
        attck = FireShot.Attack * 5;
        Destroy(gameObject, 3);
    }

    // Update is called once per frame
    void Update()
    {
        transform.Translate(speed * Time.deltaTime, 0, 0);
    }
}
