using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Wall : MonoBehaviour
{
    public GameObject Exp;
    public float wallSpeed;

    int life;
    float rate;

    // Start is called before the first frame update
    void Start()
    {
        life = 600;
        rate = 0f;

        Destroy(this.gameObject, 15);
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Bullet")
        {
            life -= FireShot.Attack;
            if (life <= 0)
            {
                Instantiate(Exp, transform.position, transform.rotation);
                GameManager.score += 600;
                Destroy(this.gameObject);
            }
        }

        if (collision.gameObject.tag == "Skill")
        {
            Instantiate(Exp, transform.position, transform.rotation);
            GameManager.score += 600;
            Destroy(this.gameObject);
        }
    }

    // Update is called once per frame
    void Update()
    {
        transform.Translate(-wallSpeed * Time.deltaTime, 0, 0);

        if (PlayerPrefs.GetInt("NowStage") == 5)
        {
            rate += Time.deltaTime / 4;

            if (rate < 0.5f)
            {
                transform.Translate(0, 1.5f * Time.deltaTime, 0);
            }
            else if (rate > 0.5f)
            {
                transform.Translate(0, -1.5f * Time.deltaTime, 0);
            }
            if (rate >= 1f)
            {
                rate = 0f;
            }
        }
    }
}
