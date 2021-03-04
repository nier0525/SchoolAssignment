using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Monster_D : MonoBehaviour
{
    public float speed;

    int hp;

    // Start is called before the first frame update
    void Start()
    {
        hp = 50;
        Destroy(gameObject, 20);
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Bullet") { 
            hp -= FireShot.Attack;

            if (hp <= 0)
            {
                gameObject.GetComponent<BoxCollider2D>().enabled = false;
                GameManager.score += 100;
            }
        }

        if (collision.gameObject.tag == "Skill")
        {
            hp -= SkillShot.attck;

            if (hp <= 0)
            {
                gameObject.GetComponent<BoxCollider2D>().enabled = false;
                GameManager.score += 100;
            }
        }

        if (collision.gameObject.tag == "Player")
        {
            Destroy(gameObject);
        }
    }

    // Update is called once per frame
    void Update()
    {
        transform.Translate(-speed * Time.deltaTime, 0, 0);

        if (hp <= 0)
        {
            transform.Translate(2 * Time.deltaTime, -15 * Time.deltaTime, 0);

            if (transform.position.y > 10)
                Destroy(gameObject);
        }
    }
}
