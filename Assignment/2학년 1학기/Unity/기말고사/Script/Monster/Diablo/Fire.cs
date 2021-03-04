using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Fire : MonoBehaviour
{
    Animator Anim;
    bool startFire=true;
    float times = 0;
    SpriteRenderer img;

    // Start is called before the first frame update
    void Start()
    {
        img = GetComponent<SpriteRenderer>();
        Anim = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        times += Time.deltaTime;
        if(times < 1.0f)
        {
            img.color = new Color(255, 255, 255, times*1.3f);
        }

       Destroy(this.gameObject, 3f);
    }
}
