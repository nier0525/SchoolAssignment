using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Monster_D3 : MonoBehaviour
{
    public float speed;

    int hp;
    float rate;

    // Start is called before the first frame update
    void Start()
    {
        hp = 150;
        rate = 0f;

        Destroy(gameObject, 20);
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Bullet")
        {
            hp -= FireShot.Attack;

            if (hp <= 0)
            {
                gameObject.GetComponent<CapsuleCollider2D>().enabled = false;
                GameManager.score += 400;
            }
        }

        if (collision.gameObject.tag == "Skill")
        {
            hp -= SkillShot.attck;

            if (hp <= 0)
            {
                gameObject.GetComponent<CapsuleCollider2D>().enabled = false;
                GameManager.score += 400;
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
        rate += Time.deltaTime / 4;

        transform.Translate(-speed * Time.deltaTime, 0, 0);

        if (rate < 0.5f)
        {
            transform.Translate(0, 1f * Time.deltaTime, 0);
        }
        else if (rate > 0.5f)
        {
            transform.Translate(0, -1f * Time.deltaTime, 0);
        }
        if (rate >= 1f)
        {
            rate = 0f;
        }

        if (hp <= 0)
        {
            transform.Translate(2 * Time.deltaTime, -15 * Time.deltaTime, 0);

            if (transform.position.y > 10)
                Destroy(gameObject);
        }
    }
}
