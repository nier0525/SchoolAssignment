using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Lazer : MonoBehaviour
{
    float speed = 5.0f;
    public float Angle = 0;
    float times = 0;
    public Vector2 playerPos;
    Vector2 thisObjPos;
    Vector2 direction;

    SpriteRenderer img;

    // Start is called before the first frame update
    void Start()
    {
        GetComponent<Animator>().SetFloat("Angle", Angle);
        thisObjPos = new Vector2(this.transform.position.x, this.transform.position.y);
        direction = (playerPos - thisObjPos).normalized;
        img = GetComponent<SpriteRenderer>();
    }

    // Update is called once per frame
    void Update()
    {
        times += Time.deltaTime;

        if(times < 1.0f)
        {
            img.color = new Color(255, 255, 255, times * 20.0f);
        }

        this.transform.Translate(direction * speed * Time.deltaTime);
        Destroy(this.gameObject, 3.0f);
    }
}
