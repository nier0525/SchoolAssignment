using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class IzualAI : MonoBehaviour
{
    public GameObject player, soul, TextB, Text;
    public GameObject Shadow;
    public SkillDatabase SkillDamege;
    public Slider HPbar;
    public Text Name;

    public AudioSource Attack, Death, GetHit, SkillSound;

    Vector2 player_pos;
    PathFinding PfScript;

    IzualMotion demon_m;
    IzualStatus demon_s;

    IEnumerator coroutineisAttak;

    public bool death = false;
    bool once2 = true;
    public bool DemonHp = false;
    bool SunBbbng = false;
    bool move = false;
    bool Exit = false;
    bool once = true;

    bool sound = true;
    bool player_die;

    Color a;

    float mana = 3f;

    // Start is called before the first frame update
    void Start()
    {
        PfScript = GetComponent<PathFinding>();

        demon_m = GetComponent<IzualMotion>();
        demon_s = GetComponent<IzualStatus>();
        GetComponent<CapsuleCollider2D>().isTrigger = false;
        a = gameObject.GetComponent<SpriteRenderer>().color;
        HPbar.gameObject.SetActive(false);
        Name.gameObject.SetActive(false);
        HPbar.maxValue = demon_s.hp;
    }

    private void Awake()
    {
        QualitySettings.vSyncCount = 0;
        Application.targetFrameRate = 60;
    }

    IEnumerator MonsterEnter()
    {
        while (true)
        {
            yield return new WaitForSeconds(1f);
            demon_m.ani.SetBool("Work", false);
            demon_m.work_speed = 0f;
        }
    }

    IEnumerator ShowText(float width, float height, string name, string text, float time)
    {
        AudioSource sound = GameObject.Find(name).GetComponent<AudioSource>();
        sound.Play();
        yield return new WaitForSeconds(1f);
        TextB.SetActive(true);
        TextB.GetComponent<RectTransform>().SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, width);
        TextB.GetComponent<RectTransform>().SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, height);

        Text.SetActive(true);
        Text.GetComponent<Text>().text = text;
        yield return new WaitForSeconds(time);
        TextB.SetActive(false);
        Text.SetActive(false);
    }

    IEnumerator AttakDelay()
    {
        while (!Exit)
        {
            gameObject.GetComponent<Rigidbody2D>().bodyType = RigidbodyType2D.Kinematic;
            gameObject.GetComponent<Rigidbody2D>().useFullKinematicContacts = true;

            yield return new WaitForSeconds(0.1f);

            if (!death && !player_die)
            {
                Shadow.GetComponent<IzualShadowMotion>().Attak(player_pos);
                demon_m.Attak(player_pos);

                yield return new WaitForSeconds(0.7f);
                Attack.Play();
                AttakDamege();
                yield return new WaitForSeconds(0.3f);
                demon_m.ani.SetBool("Attak", false);
                demon_m.ani.SetBool("Work", false);
                if (Exit)
                {
                    break;
                }
                yield return new WaitForSeconds(1f);
            }
        }
        yield return new WaitForSeconds(0.5f);
        gameObject.GetComponent<Rigidbody2D>().bodyType = RigidbodyType2D.Dynamic;
        demon_m.isAtt = false;
        move = false;
    }

    IEnumerator UsingSkill()
    {
        int index = 0;

        demon_m.isSkill = true;
        demon_m.isAtt = false;
        demon_m.isWork = false;

        demon_m.Skill(player_pos);
        SkillSound.Play();

        Color c = new Color(0, 1, 0);
        yield return new WaitForSeconds(0.3f);
        gameObject.GetComponent<SpriteRenderer>().color = c;
        yield return new WaitForSeconds(0.3f);
        gameObject.GetComponent<SpriteRenderer>().color = a;

        mana = 0f;
        demon_m.ani.SetBool("Skill", false);
        player.GetComponent<SpriteRenderer>().color = c;
        demon_m.isSkill = false;

        while (true)
        {
            yield return new WaitForSeconds(0.5f);
            player.GetComponent<PlayerState>().hp -= 4;
            index++;
            if (index >= 10)
            {
                mana = 0f;
                break;
            }
        }
        player.GetComponent<SpriteRenderer>().color = new Color(1, 1, 1);
    }

    IEnumerator HitSound()
    {
        sound = false;
        if (!death && !demon_m.isSkill && Exit)
        {
            GetHit.Play();
            demon_m.ani.SetTrigger("Hit");
        }
        yield return new WaitForSeconds(1f);
        sound = true;
    }

    IEnumerator DeathWait()
    {
        Shadow.SetActive(false);
        Death.Play();
        gameObject.GetComponent<SpriteRenderer>().sortingOrder = 0;
        gameObject.GetComponent<CircleCollider2D>().enabled = false;
        gameObject.GetComponent<CapsuleCollider2D>().enabled = false;
        Debug.Log("death");
        Quest.Next_Quest();
        yield return new WaitForSeconds(1.5f);
        soul.SetActive(true);
    }

    private void OnTriggerEnter2D(Collider2D col)
    {
        if (!death)
        {
            if (col.gameObject.tag == "Skill_blaze")
            {
                if (Exit)
                    SunBbbng = true;

                if (demon_s.hp <= 0)
                {
                    death = true;
                    demon_m.isWork = false;

                    GetComponent<CapsuleCollider2D>().isTrigger = true;
                    player.GetComponent<PlayerState>().exp += demon_s.exp;
                    demon_m.ani.SetTrigger("Death");
                    StartCoroutine(DeathWait());
                }

                else
                {
                    if (sound)
                    {
                        StartCoroutine(HitSound());
                        demon_s.hp -= SkillDamege.Blaze;
                    }
                }
            }

            if (col.gameObject.tag == "Skill_icebolt")
            {
                demon_s.hp -= SkillDamege.Icebolt;
                if (Exit)
                    SunBbbng = true;

                if (demon_s.hp <= 0)
                {
                    death = true;
                    demon_m.isWork = false;

                    GetComponent<CapsuleCollider2D>().isTrigger = true;
                    player.GetComponent<PlayerState>().exp += demon_s.exp;
                    demon_m.ani.SetTrigger("Death");
                    StartCoroutine(DeathWait());
                }

                else
                {
                    if (sound)
                        StartCoroutine(HitSound());
                }
            }

            if (col.gameObject.tag == "Skill_mfire")
            {
                if (Exit)
                    SunBbbng = true;

                if (demon_s.hp <= 0)
                {
                    death = true;
                    demon_m.isWork = false;

                    GetComponent<CapsuleCollider2D>().isTrigger = true;
                    player.GetComponent<PlayerState>().exp += demon_s.exp;
                    demon_m.ani.SetTrigger("Death");
                    StartCoroutine(DeathWait());
                }

                else
                {
                    if (sound)
                    {
                        StartCoroutine(HitSound());
                        demon_s.hp -= SkillDamege.M_fire;
                    }
                }
            }
      
            if (col.gameObject.tag == "Skill_firewall")
            {
                if (Exit)
                    SunBbbng = true;

                if (demon_s.hp <= 0)
                {
                    death = true;
                    demon_m.isWork = false;

                    GetComponent<CapsuleCollider2D>().isTrigger = true;
                    player.GetComponent<PlayerState>().exp += demon_s.exp;
                    demon_m.ani.SetTrigger("Death");
                    StartCoroutine(DeathWait());
                }

                else
                {
                    if (sound)
                    {
                        StartCoroutine(HitSound());
                        demon_s.hp -= SkillDamege.Firewall;
                    }
                }
            }
        }
    }

    void OnTriggerStay2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Layer_0")
        {
            gameObject.GetComponent<SpriteRenderer>().sortingOrder = 0;
            Shadow.GetComponent<SpriteRenderer>().sortingOrder = 0;
        }
        if (collision.gameObject.tag == "Layer_1")
        {
            gameObject.GetComponent<SpriteRenderer>().sortingOrder = 1;
            Shadow.GetComponent<SpriteRenderer>().sortingOrder = 1;
        }
        if (collision.gameObject.tag == "Layer_2")
        {
            gameObject.GetComponent<SpriteRenderer>().sortingOrder = 2;
            Shadow.GetComponent<SpriteRenderer>().sortingOrder = 2;
        }
        if (collision.gameObject.tag == "Layer_3")
        {
            gameObject.GetComponent<SpriteRenderer>().sortingOrder = 3;
            Shadow.GetComponent<SpriteRenderer>().sortingOrder = 3;
        }
    }

    void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Layer_0" || collision.gameObject.tag == "Layer_1" ||
            collision.gameObject.tag == "Layer_2" || collision.gameObject.tag == "Layer_3" ||
            collision.gameObject.tag == "Layer_4")
        {
            gameObject.GetComponent<SpriteRenderer>().sortingOrder = 5;
            Shadow.GetComponent<SpriteRenderer>().sortingOrder = 4;
        }
    }

    void OnCollisionStay2D(Collision2D col)
    {

    }

    private void OnCollisionEnter2D(Collision2D col)
    {
        if (!death)
        {
            if (col.gameObject.tag == "Skill_meteor")
            {
                demon_s.hp -= SkillDamege.Meteo;
                if (Exit)
                    SunBbbng = true;

                if (demon_s.hp <= 0)
                {
                    death = true;
                    demon_m.isWork = false;

                    GetComponent<CapsuleCollider2D>().isTrigger = true;
                    player.GetComponent<PlayerState>().exp += demon_s.exp;
                    demon_m.ani.SetTrigger("Death");
                    StartCoroutine(DeathWait());
                }

                else
                {
                    if (sound)
                        StartCoroutine(HitSound());
                }
            }         

            if (col.gameObject.tag == "Skill_blizzard")
            {
                demon_s.hp -= SkillDamege.Blizzard;
                if (Exit)
                    SunBbbng = true;

                if (demon_s.hp <= 0)
                {
                    death = true;
                    demon_m.isWork = false;

                    GetComponent<CapsuleCollider2D>().isTrigger = true;
                    player.GetComponent<PlayerState>().exp += demon_s.exp;
                    demon_m.ani.SetTrigger("Death");
                    StartCoroutine(DeathWait());
                }

                else
                {
                    if (sound)
                        StartCoroutine(HitSound());
                }
            }

            if (col.gameObject.tag == "Skill_icestorm")
            {
                demon_s.hp -= SkillDamege.Icestorm;
                if (Exit)
                    SunBbbng = true;

                if (demon_s.hp <= 0)
                {
                    death = true;
                    demon_m.isWork = false;

                    GetComponent<CapsuleCollider2D>().isTrigger = true;
                    player.GetComponent<PlayerState>().exp += demon_s.exp;
                    demon_m.ani.SetTrigger("Death");
                    StartCoroutine(DeathWait());
                }

                else
                {
                    if (sound)
                        StartCoroutine(HitSound());
                }

            }

            if (col.gameObject.tag == "Skill_fireball")
            {
                demon_s.hp -= SkillDamege.Fireball;
                if (Exit)
                    SunBbbng = true;

                if (demon_s.hp <= 0)
                {
                    death = true;
                    demon_m.isWork = false;

                    GetComponent<CapsuleCollider2D>().isTrigger = true;
                    player.GetComponent<PlayerState>().exp += demon_s.exp;
                    demon_m.ani.SetTrigger("Death");
                    StartCoroutine(DeathWait());
                }

                else
                {
                    if (sound)
                        StartCoroutine(HitSound());
                }
            }

            if (col.gameObject.tag == "Skill_firebolt")
            {
                demon_s.hp -= SkillDamege.Firebolt;
                if (Exit)
                    SunBbbng = true;

                if (demon_s.hp <= 0)
                {
                    death = true;
                    demon_m.isWork = false;

                    GetComponent<CapsuleCollider2D>().isTrigger = true;
                    player.GetComponent<PlayerState>().exp += demon_s.exp;
                    demon_m.ani.SetTrigger("Death");
                    StartCoroutine(DeathWait());
                }

                else
                {
                    if (sound)
                        StartCoroutine(HitSound());
                }
            }
        }
    }

    void OnCollisionExit2D(Collision2D col)
    {

    }

    private void OnMouseOver()
    {
        if (!death)
        {
            if (!demon_m.isSkill)
                gameObject.GetComponent<SpriteRenderer>().color = new Color(1, 1, 1, 1);
            HPbar.gameObject.SetActive(true);
            Name.gameObject.SetActive(true);
            DemonHp = true;
        }
    }

    private void OnMouseExit()
    {
        gameObject.GetComponent<SpriteRenderer>().color = a;
        HPbar.gameObject.SetActive(false);
        Name.gameObject.SetActive(false);
        DemonHp = false;
    }

    public void AttakDamege()
    {
        if (!Exit)
        {
            Debug.Log("Attak");
            player.GetComponent<PlayerState>().hp -= demon_s.Att;
            PlayerControl.HitSound = true;
        }
    }

    public void DestoryMonster()
    {
        Destroy(this.gameObject);
    }

    // Update is called once per frame
    void Update()
    {
        //Debug.Log(SkillDamege.Fireball);

        HPbar.value = demon_s.hp;
        player_pos = player.transform.position;
        float dir = Vector2.Distance(transform.position, player.transform.position);
        player_die = player.GetComponent<PlayerControl>().die;

        Vector3 TextPos = new Vector3(this.transform.position.x, this.transform.position.y + 0.6f, 0);
        TextB.transform.position = Camera.main.WorldToScreenPoint(TextPos);
        Text.transform.position = Camera.main.WorldToScreenPoint(TextPos);

        if ((dir >= 0.8f && dir < 6f || SunBbbng) && !player_die)
        {
            PfScript.Raycasting(player_pos);

            Exit = true;
            move = false;

            if (!death && !demon_m.isAtt && !demon_m.isSkill)
            {
                if (!PfScript.isMove)
                    demon_m.Work(player_pos);
                else
                {
                    demon_m.ani.SetBool("Work", true);

                    PfScript.MoveToNode(demon_m.work_speed, demon_m.ani, 1 , 0);
                }

                mana += Time.deltaTime / 2;

                if (mana >= 6f)
                {
                    StartCoroutine(UsingSkill());
                }
            }
            else
            {
                demon_m.ani.SetBool("Work", false);
            }
        }

        if (dir < 3f && !player_die)
        {
            if (once)
            {
                once = false;
                StartCoroutine(ShowText(120f, 20f, "izualtaunt1", "네 자신이나 구해라!!", 4f));
            }
        }

        if (dir < 0.8f)
        {
            demon_m.ani.SetBool("Work", false);

            if (!move)
            {
                PfScript.isMove = false;
                PfScript.isWall = false;

                Exit = false;
                move = true;
                coroutineisAttak = AttakDelay();
                StartCoroutine(coroutineisAttak);
            }

            SunBbbng = false;
            demon_m.isAtt = true;
        }

        if (dir > 6f && !death && !player_die && !SunBbbng)
        {
            PfScript.isMove = false;
            PfScript.isWall = false;

            demon_m.ani.SetBool("Work", false);
            demon_m.isWork = false;
        }

        if (player_die)
        {
            demon_m.isAtt = false;
            demon_m.isWork = false;
            demon_m.ani.SetBool("Work", false);
            //StopCoroutine(coroutineisAttak);
            gameObject.GetComponent<Rigidbody2D>().bodyType = RigidbodyType2D.Dynamic;
        }

        if (death && once2)
        {
            once2 = false;
            player.GetComponent<PlayerControl>().izualdeath = true;
        }
    }
}

