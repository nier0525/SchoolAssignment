using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class test : MonoBehaviour
{
    //float h, v;
    float speed;
    float x, y;
    int num;

    bool move = false;
    bool wait = true;

    Vector3 targetpos;

    GameObject roof, rooftile;
    GameObject obj , obj2, obj3;

    SpriteRenderer[] obj_spr, obj2_spr, obj3_spr;
    Rigidbody2D rd;

    public GameObject loading;

    // Start is called before the first frame update
    void Start()
    {
        roof = GameObject.Find("FortressRoof");
        rooftile = GameObject.Find("Fortress Roofs");

        obj = GameObject.Find("FortressObject2");
        obj_spr = obj.gameObject.GetComponentsInChildren<SpriteRenderer>();

        obj2 = GameObject.Find("alpha01");
        obj2_spr = obj2.gameObject.GetComponentsInChildren<SpriteRenderer>();

        obj3 = GameObject.Find("alpha02");
        obj3_spr = obj3.gameObject.GetComponentsInChildren<SpriteRenderer>();

        rd = GetComponent<Rigidbody2D>();
        GameObject.Find("TownBGM").GetComponent<AudioSource>().Play();

        speed = 2f;
        x = y = 0;
        //h = v = 0;
    }

    void LoadingWait()
    {
        wait = false;
        loading.SetActive(true);
        GameObject.Find("Cursor").GetComponent<Curser>().Waiting = true;
        StartCoroutine("LoadingWaitTime");
    }

    IEnumerator LoadingWaitTime()
    {
        num = 0;

        while (!wait)
        {
            if (num > 3)
            {
                loading.SetActive(false);
                GameObject.Find("Cursor").GetComponent<Curser>().Waiting = false;
                wait = true;
            }
            num++;    
            yield return new WaitForSeconds(0.50f);
        }
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        // 포탈
        //------------------------------------------------------------------------
        {
            if (collision.gameObject.tag == "Gate01")
            {
                move = false;
                LoadingWait();

                x = GameObject.Find("zone01").transform.position.x;
                y = GameObject.Find("zone01").transform.position.y;

                transform.position = new Vector2(x, y);
            }

            if (collision.gameObject.tag == "Gate02")
            {
                move = false;
                GameObject.Find("HellBGM").GetComponent<AudioSource>().Stop();
                LoadingWait();
                GameObject.Find("DiabloBGM").GetComponent<AudioSource>().Play();

                x = GameObject.Find("zone02").transform.position.x;
                y = GameObject.Find("zone02").transform.position.y;

                transform.position = new Vector2(x, y);
            }

            if (collision.gameObject.name == "Gate03")
            {
                move = false;
                GameObject.Find("TownBGM").GetComponent<AudioSource>().Stop();
                LoadingWait();
                GameObject.Find("HellBGM").GetComponent<AudioSource>().Play();

                x = GameObject.Find("zone03").transform.position.x;
                y = GameObject.Find("zone03").transform.position.y;

                transform.position = new Vector2(x, y);
            }
        }
        //------------------------------------------------------------------------

        // town 건물 투명화
        //------------------------------------------------------------------------
        {
            if (collision.gameObject.name == "RoofIn")
            {
                roof.SetActive(false);
                rooftile.SetActive(false);

                Color s = new Color(1, 1, 1, 0.3f);

                for (int i = 0; i < obj_spr.Length; i++)
                {
                    obj_spr[i].color = s;
                }
            }

            if (collision.gameObject.name == "roofin2")
            {
                Color s = new Color(1, 1, 1, 0.3f);
                for (int i = 0; i < obj2_spr.Length; i++)
                {
                    obj2_spr[i].color = s;
                }
            }

            if (collision.gameObject.name == "roofin3")
            {
                Color s = new Color(1, 1, 1, 0.3f);
                for (int i = 0; i < obj3_spr.Length; i++)
                {
                    obj3_spr[i].color = s;
                }
            }

            if (collision.gameObject.name == "RoofOut")
            {
                roof.SetActive(true);
                rooftile.SetActive(true);

                Color s = new Color(1, 1, 1, 1f);

                for (int i = 0; i < obj_spr.Length; i++)
                {
                    obj_spr[i].color = s;
                }
            }

            if (collision.gameObject.name == "roofout2")
            {
                Color s = new Color(1, 1, 1, 1f);
                for (int i = 0; i < obj2_spr.Length; i++)
                {
                    obj2_spr[i].color = s;
                }
            }

            if (collision.gameObject.name == "roofout3")
            {
                Color s = new Color(1, 1, 1, 1f);
                for (int i = 0; i < obj3_spr.Length; i++)
                {
                    obj3_spr[i].color = s;
                }
            }
        }
        //------------------------------------------------------------------------
    }

    private void OnTriggerStay2D(Collider2D collision)
    {
        // 캐릭터 이동 관련
        //------------------------------------------------------------------------
        if (collision.gameObject.tag == "npc")
        {
            move = false;
        }
        //------------------------------------------------------------------------
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        //h = Input.GetAxis("Horizontal") * speed * Time.deltaTime;
        //v = Input.GetAxis("Vertical") * speed * Time.deltaTime;

        //transform.Translate(h, v, 0);

        if (wait)
        {
            if (Input.GetKeyDown(KeyCode.R))
            {
                move = false;

                x = GameObject.Find("town").transform.position.x;
                y = GameObject.Find("town").transform.position.y;

                transform.position = new Vector2(x, y);
            }

            if (Input.GetMouseButton(0))
            {
                targetpos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
                Ray2D ray = new Ray2D(targetpos, Vector2.zero);
                RaycastHit2D hit = Physics2D.Raycast(ray.origin, ray.direction);

                move = true;
            }
        }

        if (move)
        {
            transform.localPosition = Vector2.MoveTowards(transform.position, targetpos, speed * Time.deltaTime);
            float dis = Vector2.Distance(transform.position, targetpos);
            if (dis < 0.01f)
            {
                move = false;
            }
        }
    }
}
