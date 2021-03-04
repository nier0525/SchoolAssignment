using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FireShot : MonoBehaviour
{
    AudioSource SFX;

    public float Speed;
    static public int Attack;

    bool move = true;

    // Start is called before the first frame update
    void Start()
    {
        SFX = GameObject.Find("Boom").GetComponent<AudioSource>();

        Attack = 50;
        Destroy(gameObject, 15);
    }

    IEnumerator Boom()
    {
        move = false;
        this.GetComponent<Animator>().SetTrigger("Boom");
        SFX.Play();
        yield return new WaitForSeconds(0.4f);
        Destroy(gameObject);
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag != "BossSkill" && collision.gameObject.tag != "Bullet")
        {
            StartCoroutine(Boom());
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (move)
        {
            transform.Translate(Speed * Time.deltaTime, 0, 0);
        }
    }
}
