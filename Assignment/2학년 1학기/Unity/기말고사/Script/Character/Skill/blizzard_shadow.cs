using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class blizzard_shadow : MonoBehaviour
{
    private SpriteRenderer fade;
    float fades = 0.0f;

    Vector2 mousePos;

    // Use this for initialization
    void Start()
    {
        fade = GetComponent<SpriteRenderer>();
        mousePos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
    }

    // Update is called once per frame
    void Update()
    {
        transform.position = Vector2.MoveTowards(transform.position, mousePos, 1.5f * Time.deltaTime);
        if(fades<1.0f)
        {
            fades += 0.01f;
            fade.color = new Color(0, 0, 0, fades);
        }

        if (transform.position.x == mousePos.x && transform.position.y == mousePos.y)
        {
            Destroy(this.gameObject);
        }
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Monster")
        {
            Destroy(this.gameObject);
            Debug.Log("충돌");
        }
    }
}

