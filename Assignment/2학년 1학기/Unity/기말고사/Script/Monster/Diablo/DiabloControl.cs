using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class DiabloControl : DiabloMotion
{
    Animator Anim;
    Vector2 playerPos;
    PlayerState Player_s;
    PlayerControl Player_c;
    public GameObject Player;
    public GameObject Lazer;
    public GameObject Fire;
    public GameObject SkillDatabase;
    public GameObject TextB;
    public GameObject Text;
    public GameObject Shadow;
    PathFinding PfScript;

    CapsuleCollider2D collider1;
    CircleCollider2D collider2;
    Rigidbody2D rigid;
    SkillDatabase SkillDB;
    SpriteRenderer Img;

    IEnumerator StopInv;

    public Slider HPbar;
    bool HP;
    public Text Name;
    Color a;

    float distance;
    public float Angle;
    int Randnum;
    int prevnum = -1;
    int Idlenum;

    bool isWalk = true;
    bool isRun = true;
    bool stillWalk = true;
    bool isAttack = false;
    bool isWait = true;
    bool createRandnum = true;
    public bool death = false;
    bool isDeath = true;
    bool isHit = false;

    int soundNum;

    float times = 0;

    public bool AttackStart = false;
    //  About Diablo Status
    float MaxHp;
    float hp;
    int AttackDmg = 10;

    // About Diablo Phase
    bool Phase1 = true;
    bool Phase2 = false;
    bool Phase3 = false;
    bool baseSet = true;

    // 페이즈2
    bool startP2Event = true;
    bool startP2 = false;
    bool useLazer = true;
    bool useFire = true;


    //  페이즈3
    bool stopLazer = false;
    bool startP3Event = true;
    bool startP3 = false;
    bool startInv = false;
    bool startAttack = false;
    public int InvAttackCount = 0;

    // 죽음
    bool DeathBodySound = true;

    IEnumerator playerspeech;

    //그림자
    public bool s_Attack = false;
    public bool s_Walk = false;
    public bool s_Run = false;
    public bool s_RunAndAttack = false;
    public bool s_UseLazer = false;
    public bool s_UseFire = false;
    public bool s_UseCold = false;
    public bool s_UseInv = false;

    public GameObject FieldPortal;
    public GameObject TownPortal;

    // Start is called before the first frame update
    void Start()
    {
        PfScript = GetComponent<PathFinding>();

        Player_c = Player.GetComponent<PlayerControl>();

        playerspeech = Player.GetComponent<PlayerControl>().ShowText(180f, 20f, "killDiablo", "공포는 더이상 지옥을 활보하지 않는다.", 5f);

        MaxHp = 60000;
        hp = 60000;

        Anim = GetComponent<Animator>();
        collider1 = GetComponent<CapsuleCollider2D>();
        collider2 = GetComponent<CircleCollider2D>();
        rigid = GetComponent<Rigidbody2D>();
        Player_s = Player.GetComponent<PlayerState>();
        SkillDB = SkillDatabase.GetComponent<SkillDatabase>();
        Img = GetComponent<SpriteRenderer>();

        a = gameObject.GetComponent<SpriteRenderer>().color;
        HPbar.gameObject.SetActive(false);
        Name.gameObject.SetActive(false);
        HPbar.maxValue = hp;
    }

    private void Awake()
    {
        QualitySettings.vSyncCount = 0;
        Application.targetFrameRate = 60;
    }

    void BaseSetting()
    {
        isRun = true;
        isAttack = false;
        useFire = true;
        createRandnum = true;
        Anim.SetBool("Walk", false);
        Anim.SetBool("Run", false);
        Anim.SetBool("Attack", false);
        Anim.SetBool("RunAttack", false);
        Anim.SetBool("UseFire", false);
        Anim.SetBool("UseCold", false);
        Anim.SetBool("UseLazer", false);

        s_Attack = false;
        s_Walk = false;
        s_Run = false;
        s_RunAndAttack = false;
        s_UseLazer = false;
        s_UseFire = false;
        s_UseCold = false;
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 TextPos = new Vector3(this.transform.position.x, this.transform.position.y + 0.7f, 0);
        TextB.transform.position = Camera.main.WorldToScreenPoint(TextPos);
        Text.transform.position = Camera.main.WorldToScreenPoint(TextPos);

        HPbar.value = hp;

        playerPos = Player.transform.position;
        distance = Vector2.Distance(playerPos, this.transform.position);
        Angle = GetDegree(this.transform.position, playerPos);

        PhaseTrigger();

        //PathFind
        PfScript.Raycasting(playerPos);

        if (!Player_c.die)  //플레이어 사망 아닐 시
        {
            if (!death)
            {
                if (Phase1)
                {
                    OnPhase1();
                }
                if (Phase2)
                {
                    if (startP2)
                    {
                        OnPhase2();
                    }
                }
                if (Phase3)
                {
                    if (startP3)
                    {
                        OnPhase3();
                    }
                }
            }
            else
            {
                if (isDeath)
                {
                    AudioSource lazer = GameObject.Find("uselazer").GetComponent<AudioSource>();
                    lazer.Stop();
                    stopLazer = true;

                    Quest.Next_Quest();
                    SpriteRenderer sr = GetComponent<SpriteRenderer>();
                    if (sr.sortingOrder == 5)
                    {
                        sr.sortingOrder = 3;
                    }
                    Color color = sr.color;

                    color.a = 1f;
                    sr.color = color;

                    StopAllCoroutines();
                    Anim.SetTrigger("Death");
                    if (DeathBodySound)
                    {
                        DeathBodySound = false;
                        StartCoroutine(deathbodySound());
                    }
                    AudioSource sound = GameObject.Find("diablo_death").GetComponent<AudioSource>();
                    sound.Play();
                    collider1.enabled = false;
                    collider2.enabled = false;
                    isDeath = false;

                    //플레이어 콜드 상태 해제
                    PlayerMotion Player_m = Player.GetComponent<PlayerMotion>();
                    SpriteRenderer Img = Player.GetComponent<SpriteRenderer>();

                    Player_m.walk_speed = 1.5f;
                    Player_m.run_speed = 1.7f;
                    Player.GetComponent<Animator>().SetFloat("walkSpeed", 1.2f);
                    Player.GetComponent<Animator>().SetFloat("runSpeed", 1.2f);
                    Img.color = new Color(255, 255, 255);

                    StartCoroutine(OpenPortal());
                }
            }
        }
        else
        {
            StopAllCoroutines();
            BaseSetting();
        }
    }

    IEnumerator OpenPortal()
    {
        yield return new WaitForSeconds(11f);
        FieldPortal.SetActive(true);
        TownPortal.SetActive(true);
    }

    private void OnMouseEnter()
    {
        if (!death && !startInv)
        {
            gameObject.GetComponent<SpriteRenderer>().color = new Color(1, 1, 1, 1);
            HPbar.gameObject.SetActive(true);
            Name.gameObject.SetActive(true);
            HP = true;
        }
    }

    private void OnMouseExit()
    {
        if (!death && !startInv)
        {
            gameObject.GetComponent<SpriteRenderer>().color = a;
            HPbar.gameObject.SetActive(false);
            Name.gameObject.SetActive(false);
            HP = false;
        }
    }

    IEnumerator deathbodySound()
    {
        yield return new WaitForSeconds(8.5f);
        AudioSource sound = GameObject.Find("deathhit").GetComponent<AudioSource>();
        sound.Play();
        if (gameObject.GetComponent<SpriteRenderer>().sortingOrder == 5)
        {
            gameObject.GetComponent<SpriteRenderer>().sortingOrder = 3;
            Shadow.GetComponent<SpriteRenderer>().sortingOrder = 2;
        }
        StartCoroutine(playerspeech);
    }

    void PhaseTrigger()
    {
        if (hp <= 0)
            death = true;

        if (!death)
        {
            if (MaxHp >= hp && hp > MaxHp / 100 * 70)
            {
                Phase1 = true;
                Phase2 = false;
                Phase3 = false;
            }
            if (MaxHp / 100 * 70 >= hp && hp > MaxHp / 100 * 30)
            {
                if (baseSet)
                {
                    baseSet = false;
                    BaseSetting();
                    StartCoroutine(StartPhase2());
                }
                Phase1 = false;
                Phase2 = true;
                Phase3 = false;
            }
            if (MaxHp / 100 * 30 >= hp && hp > 0)
            {
                if (!baseSet)
                {
                    baseSet = true;
                    BaseSetting();
                    StartCoroutine(StartPhase3());
                }
                Phase1 = false;
                Phase2 = false;
                Phase3 = true;
            }
        }
    }

    //  페이즈1
    void OnPhase1()
    {
        CreateRandNum(0, 5);        //난수 생성
        if (1.2f <= distance && distance < 10f)
        {
            rigid.bodyType = RigidbodyType2D.Dynamic;

            if (Randnum < 4)        // 걷기
            {
                if (isWalk)
                {
                    isAttack = false;

                    if (!PfScript.isMove)
                    {
                        Walk(Anim, playerPos, Angle);
                        s_Walk = true;
                    }
                    else
                    {
                        Anim.SetBool("Run", false);
                        Anim.SetBool("Walk", true);
                        s_Run = false;
                        s_Walk = true;

                        PfScript.MoveToNode(WalkSpeed, Anim, 0, 0);
                    }

                    if (stillWalk)
                    {
                        StartCoroutine(WalktoRun());
                    }
                }
            }
            else if (Randnum == 4)  // 달리기
            {
                if (isRun)
                {
                    isAttack = false;

                    if (!PfScript.isMove)
                    {
                        Run(Anim, playerPos, Angle);
                        s_Run = true;
                    }
                    else
                    {
                        Anim.SetFloat("Angle", Angle);
                        Anim.SetBool("Run", true);
                        Anim.SetBool("Walk", false);
                        s_Run = true;
                        s_Walk = false;

                        PfScript.MoveToNode(RunSpeed, Anim, 0, 0);
                    }
                }
            }
            else if (Randnum == 5)  // 아이들
            {
                if (isWait)
                {
                    isAttack = false;
                    isWait = false;
                    StartCoroutine(Idle());
                }
            }
        }
        else if (1.2f > distance)
        {
            rigid.bodyType = RigidbodyType2D.Kinematic;
            rigid.useFullKinematicContacts = true;

            if (Randnum < 4)
            {
                if (!isAttack)
                {
                    isWalk = false;
                    isAttack = true;
                    StartCoroutine(AttackDelay());
                }
            }
            else if (Randnum == 4)
            {
                if (!isAttack)
                {
                    isRun = false;
                    isAttack = true;
                    StartCoroutine(RunAttackDelay());
                }
            }
        }
    }

    //  페이즈2
    void OnPhase2()
    {
        CreateRandNum(0, 4);
        if (Randnum == 0)    //레이저
        {
            if (useLazer)
            {
                useLazer = false;
                StartCoroutine(UseLazer());
            }
        }
        else if (Randnum == 1)   //파이어
        {
            if (useFire)
            {
                useFire = false;
                StartCoroutine(UseFire());
            }
        }
        else if (Randnum >= 2)    // 2, 3
        {
            if (1.2f <= distance)
            {
                if (isRun)
                {
                    isAttack = false;

                    if (!PfScript.isMove)
                    {
                        Run(Anim, playerPos, Angle);
                        s_Run = true;
                    }
                    else
                    {
                        Anim.SetFloat("Angle", Angle);
                        Anim.SetBool("Run", true);
                        Anim.SetBool("Walk", false);

                        s_Run = true;
                        s_Walk = false;

                        PfScript.MoveToNode(RunSpeed, Anim, 0, 0);
                    }
                }
            }
            else if (1.2f > distance)
            {
                if (Randnum == 2)   //콜드
                {
                    if (!isAttack)
                    {
                        isRun = false;
                        isAttack = true;
                        StartCoroutine(UseCold());
                    }
                }
                else if (Randnum == 3)  //기본 공격
                {
                    if (!isAttack)
                    {
                        isRun = false;
                        isAttack = true;
                        StartCoroutine(RunAttackDelay());
                    }
                }
            }
        }
    }

    // 페이즈3
    void OnPhase3()
    {
        CreateRandNum(0, 4);
        if (Randnum == 0)   //파이어
        {
            if (useFire)
            {
                useFire = false;

                StopInv = InvisibleAttack();
                StopCoroutine(StopInv);

                OnVisible();

                StartCoroutine(P3_UseFire());
            }
        }
        else if (Randnum == 1)  //투명화
        {
            if (!startInv)
            {
                startInv = true;
                StopInv = InvisibleAttack();
                StartCoroutine(StartInvisible());
            }

            if (startAttack)
            {
                if (1.2f <= distance)
                {
                    rigid.bodyType = RigidbodyType2D.Dynamic;
                    if (isRun)
                    {
                        isAttack = false;
                        if (!PfScript.isMove)
                        {
                            Run(Anim, playerPos, Angle);
                            s_Run = true;
                        }
                        else
                        {
                            Anim.SetFloat("Angle", Angle);
                            Anim.SetBool("Run", true);
                            Anim.SetBool("Walk", false);
                            s_Run = true;
                            s_Walk = false;

                            PfScript.MoveToNode(RunSpeed, Anim, 0, 0);
                        }
                    }
                }
                else if (1.2f > distance)
                {
                    rigid.bodyType = RigidbodyType2D.Kinematic;
                    rigid.useFullKinematicContacts = true;
                    if (!isAttack)
                    {
                        isRun = false;
                        isAttack = true;
                        StartCoroutine(InvisibleAttack());
                    }
                }
            }
        }
        else if (Randnum == 2)      // 콜드
        {
            StopInv = InvisibleAttack();
            StopCoroutine(StopInv);

            OnVisible();

            if (1.2f <= distance)
            {
                rigid.bodyType = RigidbodyType2D.Dynamic;
                if (isRun)
                {
                    isAttack = false;
                    Run(Anim, playerPos, Angle);
                    s_Run = true;
                }
            }
            else if (1.2f > distance)
            {
                rigid.bodyType = RigidbodyType2D.Kinematic;
                rigid.useFullKinematicContacts = true;
                if (!isAttack)
                {
                    isRun = false;
                    isAttack = true;
                    StartCoroutine(UseCold());
                }
            }
        }
        else if (Randnum == 3)      // 레이저
        {
            StopInv = InvisibleAttack();
            StopCoroutine(StopInv);

            OnVisible();

            if (useLazer)
            {
                if (useLazer)
                {
                    useLazer = false;
                    StartCoroutine(UseLazer());
                }
            }
        }
    }

    void OnVisible()
    {
        SpriteRenderer Spr = GetComponent<SpriteRenderer>();
        Color Color = Spr.color;
        Color.a = 1f;
        Spr.color = Color;
    }

    AudioSource RandomSound(string type1, string type2, string type3)
    {
        GameObject obj = null;
        AudioSource sound;

        soundNum = Random.Range(0, 3);
        switch (soundNum)
        {
            case 0:
                obj = GameObject.Find(type1);
                break;
            case 1:
                obj = GameObject.Find(type2);
                break;
            case 2:
                obj = GameObject.Find(type3);
                break;
        }
        sound = obj.GetComponent<AudioSource>();

        return sound;
    }

    void CreateRandNum(int n1, int n2)
    {
        if (createRandnum)
        {
            //Debug.Log("Randnum: " + Randnum);
            while (true)
            {
                Randnum = Random.Range(n1, n2);
                Idlenum = Random.Range(0, 4);
                if (Randnum != prevnum)
                    break;
            }
            prevnum = Randnum;
            //Debug.Log("prevnum: " + prevnum);
        }
        createRandnum = false;
    }

    IEnumerator StartPhase2()
    {
        collider1.enabled = false;
        collider2.enabled = false;
        SpriteRenderer img = GetComponent<SpriteRenderer>();

        AudioSource sound = GameObject.Find("diablotaunt2").GetComponent<AudioSource>();
        sound.Play();

        TextB.SetActive(true);
        TextB.GetComponent<RectTransform>().SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, 150f);
        TextB.GetComponent<RectTransform>().SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, 20f);

        Text.SetActive(true);
        Text.GetComponent<Text>().text = "...생명의 냄새가 날 둘러싼다.";

        yield return new WaitForSeconds(5f);
        TextB.SetActive(false);
        Text.SetActive(false);

        collider1.enabled = true;
        collider2.enabled = true;
        startP2 = true;
    }

    IEnumerator StartPhase3()
    {
        collider1.enabled = false;
        collider2.enabled = false;
        SpriteRenderer img = GetComponent<SpriteRenderer>();

        stopLazer = true;
        AudioSource sound = GameObject.Find("diablotaunt1").GetComponent<AudioSource>();
        sound.Play();

        TextB.SetActive(true);
        TextB.GetComponent<RectTransform>().SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, 150f);
        TextB.GetComponent<RectTransform>().SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, 20f);

        Text.SetActive(true);
        Text.GetComponent<Text>().text = "넌 설령 죽어도 내게서 도망치지 못한다.";
        yield return new WaitForSeconds(5f);
        TextB.SetActive(false);
        Text.SetActive(false);

        collider1.enabled = true;
        collider2.enabled = true;
        startP3 = true;
        stopLazer = false;
    }

    //투명화
    IEnumerator StartInvisible()
    {
        collider1.enabled = false;
        collider2.enabled = false;

        AudioSource sound = GameObject.Find("laugh1").GetComponent<AudioSource>();
        sound.Play();

        SpriteRenderer sr = GetComponent<SpriteRenderer>();
        Color color = sr.color;

        Anim.SetBool("UseInv", true);
        s_UseInv = true;

        for (float i = 1f; i >= 0; i -= 0.05f)
        {
            color.a = i;
            sr.color = color;
            yield return new WaitForSeconds(0.1f);
        }
        color.a = 0f;
        sr.color = color;

        yield return new WaitForSeconds(0.5f);
        Anim.SetBool("UseInv", false);
        s_UseInv = false;

        yield return new WaitForSeconds(1.5f);

        collider1.enabled = true;
        collider2.enabled = true;
        startAttack = true;
        InvAttackCount = 0;
    }

    //투명화상태 공격
    IEnumerator InvisibleAttack()
    {
        Anim.SetBool("Run", false);
        s_Run = false;

        SpriteRenderer sr = GetComponent<SpriteRenderer>();
        Color color = sr.color;

        while (1.2f > distance)
        {
            if (InvAttackCount >= 5)
            {
                Debug.Log("투명공격 브레이크");
                break;
            }

            InvAttackCount++;
            AudioSource sound = RandomSound("dia_attack1", "dia_attack2", "dia_attack3");

            if (!isAttack)
            {
                break;
            }

            AttackStart = true;

            for (float i = 0f; i <= 1f; i += 0.1f)
            {
                color.a = i;
                sr.color = color;
                yield return new WaitForSeconds(0.01f);
            }

            Attack(Anim, playerPos, Angle);
            s_Attack = true;
            sound.Play();
            yield return new WaitForSeconds(0.5f);
            Anim.SetBool("Attack", false);
            s_Attack = false;

            if (GetComponentInChildren<HitZone>().isHit && !death)
            {
                sound = RandomSound("hit1", "hit2", "hit3");
                sound.Play();
                if (Player_c.ManaShield)
                {
                    Player_s.mp -= AttackDmg;
                }
                else
                    Player_s.hp -= AttackDmg;
            }

            for (float i = 1f; i >= 0; i -= 0.1f)
            {
                color.a = i;
                sr.color = color;
                yield return new WaitForSeconds(0.01f);
            }
            color.a = 0f;
            sr.color = color;

            yield return new WaitForSeconds(0.7f);

            AttackStart = false;
        }

        rigid.bodyType = RigidbodyType2D.Dynamic;
        yield return new WaitForSeconds(0.5f);

        isRun = true;
        if (InvAttackCount >= 5)
        {
            for (float i = 0f; i <= 1f; i += 0.1f)
            {
                color.a = i;
                sr.color = color;
                yield return new WaitForSeconds(0.01f);
            }

            DiabloShadow DShadow = GetComponentInChildren<DiabloShadow>();
            SpriteRenderer ShadowSpr = GameObject.Find("DiabloShadow").GetComponent<SpriteRenderer>();
            ShadowSpr.color = DShadow.Original_color;

            Anim.SetBool("Run", false);
            s_Run = false;
            startInv = false;
            startAttack = false;
            createRandnum = true;

            yield return new WaitForSeconds(1.5f);
        }
    }

    // 페이즈3 파이어
    IEnumerator P3_UseFire()
    {
        rigid.bodyType = RigidbodyType2D.Kinematic;
        rigid.useFullKinematicContacts = true;

        int index = 5;
        float speed = 130.0f;

        AudioSource sound = GameObject.Find("castfire").GetComponent<AudioSource>();

        yield return new WaitForSeconds(1f);

        int count = 0;
        int angle = 0;
        while (count < 10)
        {
            UseFire(Anim, playerPos, Angle);
            s_UseFire = true;
            sound.Play();
            yield return new WaitForSeconds(0.5f);

            for (int i = 0; i < index; i++)
            {
                GameObject item = (GameObject)Instantiate(Fire, this.transform.position, Quaternion.identity);
                item.GetComponent<Rigidbody2D>().AddForce(new Vector2(speed * Mathf.Cos(Mathf.PI * 2 * i / index + angle), speed * Mathf.Sin(Mathf.PI * i * 2 / index + angle)));
            }
            Anim.SetBool("UseFire", false);
            s_UseFire = false;

            count++;
            index += 2;
            angle += 60;
            yield return new WaitForSeconds(0.4f);
        }

        yield return new WaitForSeconds(1.5f);

        rigid.bodyType = RigidbodyType2D.Dynamic;

        useFire = true;
        createRandnum = true;
    }

    IEnumerator P3_UseFire2()
    {
        yield return new WaitForSeconds(1.5f);
    }

    //  레이저
    IEnumerator UseLazer()
    {
        rigid.bodyType = RigidbodyType2D.Kinematic;
        rigid.useFullKinematicContacts = true;
        int index = 0;

        yield return new WaitForSeconds(1f);
        UseLazer(Anim, playerPos, Angle);
        s_UseLazer = true;
        AudioSource sound = GameObject.Find("castlazer").GetComponent<AudioSource>();
        sound.Play();

        sound = GameObject.Find("uselazer").GetComponent<AudioSource>();
        sound.Play();
        while (index < 40)
        {
            if (stopLazer)
                break;

            Anim.SetFloat("Angle", Angle);
            yield return new WaitForSeconds(0.1f);

            GameObject item = Instantiate(Lazer) as GameObject;
            item.GetComponent<Lazer>().Angle = Angle;
            item.GetComponent<Lazer>().playerPos = playerPos;
            Vector3 temp = Player.transform.position - this.transform.position;
            float distance = temp.magnitude;
            Vector3 direction = temp / distance;
            item.transform.position = this.transform.position + (direction * 1.1f);

            index++;
        }
        Anim.SetBool("UseLazer", false);
        s_UseLazer = false;
        sound.Stop();

        yield return new WaitForSeconds(1.5f);

        rigid.bodyType = RigidbodyType2D.Dynamic;

        useLazer = true;
        createRandnum = true;
    }

    //  불 장막
    IEnumerator UseFire()
    {
        rigid.bodyType = RigidbodyType2D.Kinematic;
        rigid.useFullKinematicContacts = true;

        int index = 30;
        float angle = 360 / index;
        float speed = 150.0f;

        AudioSource sound = GameObject.Find("castfire").GetComponent<AudioSource>();

        yield return new WaitForSeconds(1f);
        UseFire(Anim, playerPos, Angle);
        s_UseFire = true;
        sound.Play();
        yield return new WaitForSeconds(0.5f);

        for (int i = 0; i < index; i++)
        {
            GameObject item = (GameObject)Instantiate(Fire, this.transform.position, Quaternion.identity);
            item.GetComponent<Rigidbody2D>().AddForce(new Vector2(speed * Mathf.Cos(Mathf.PI * 2 * i / index), speed * Mathf.Sin(Mathf.PI * i * 2 / index)));
        }
        Anim.SetBool("UseFire", false);
        s_UseFire = false;

        yield return new WaitForSeconds(1.5f);

        rigid.bodyType = RigidbodyType2D.Dynamic;

        useFire = true;
        createRandnum = true;
    }

    // 콜드
    IEnumerator UseCold()
    {
        AudioSource sound = GameObject.Find("castcold").GetComponent<AudioSource>();

        rigid.bodyType = RigidbodyType2D.Kinematic;
        rigid.useFullKinematicContacts = true;

        Anim.SetBool("Run", false);
        s_Run = false;
        UseCold(Anim, playerPos, Angle);
        s_UseCold = true;
        yield return new WaitForSeconds(0.1f);
        sound.Play();
        yield return new WaitForSeconds(0.6f);
        Anim.SetBool("UseCold", false);
        s_UseCold = false;
        StartCoroutine(ChangePlayerColor());
        sound = RandomSound("hit1", "hit2", "hit3");
        sound.Play();
        if (Player_c.ManaShield)
        {
            Player_s.mp -= AttackDmg;
        }
        else
            Player_s.hp -= AttackDmg;
        yield return new WaitForSeconds(1f);
        rigid.bodyType = RigidbodyType2D.Dynamic;

        isRun = true;
        isAttack = false;
        createRandnum = true;
    }

    IEnumerator ChangePlayerColor()
    {
        PlayerMotion Player_m = Player.GetComponent<PlayerMotion>();
        SpriteRenderer Img = Player.GetComponent<SpriteRenderer>();

        Player_m.walk_speed = 0.5f;
        Player_m.run_speed = 1.0f;
        Player.GetComponent<Animator>().SetFloat("walkSpeed", 0.3f);
        Player.GetComponent<Animator>().SetFloat("runSpeed", 0.3f);
        Img.color = new Color(0, 0, 255);
        yield return new WaitForSeconds(6f);
        Player_m.walk_speed = 1.5f;
        Player_m.run_speed = 1.7f;
        Player.GetComponent<Animator>().SetFloat("walkSpeed", 1.2f);
        Player.GetComponent<Animator>().SetFloat("runSpeed", 1.2f);
        Img.color = new Color(255, 255, 255);
    }

    //  아이들
    IEnumerator Idle()
    {
        Anim.SetBool("Walk", false);
        s_Walk = false;
        yield return new WaitForSeconds(1f);
        isWait = true;
        createRandnum = true;
    }

    //  걷기 -> 달리기
    IEnumerator WalktoRun()
    {
        stillWalk = false;
        if (Idlenum == 1)
        {
            yield return new WaitForSeconds(2f);
            Randnum = 5;
            stillWalk = true;
        }
        else
        {
            Debug.Log("코루틴 시작");
            yield return new WaitForSeconds(3f);
            Debug.Log(Phase1);

            if (!isAttack && Phase1)
            {
                Debug.Log("뜀");
                Anim.SetBool("Walk", false);
                s_Walk = false;
                Randnum = 4;
                stillWalk = true;
            }
        }
    }

    // 달리기 공격
    IEnumerator RunAttackDelay()
    {
        if (isAttack)
        {
            AudioSource sound = RandomSound("dia_attack1", "dia_attack2", "dia_attack3");

            Anim.SetBool("Run", false);
            s_Run = false;
            RunAndAttack(Anim, playerPos, Angle);
            s_RunAndAttack = true;
            sound.Play();
            yield return new WaitForSeconds(0.3f);
            Anim.SetBool("RunAttack", false);
            s_RunAndAttack = false;

            if (GetComponentInChildren<HitZone>().isHit && !death)
            {
                sound = RandomSound("hit1", "hit2", "hit3");
                sound.Play();
                if (Player_c.ManaShield)
                {
                    Player_s.mp -= AttackDmg;
                }
                else
                    Player_s.hp -= AttackDmg;
            }

            yield return new WaitForSeconds(1.5f);
        }

        isRun = true;
        isAttack = false;
        if (Phase1)
            Randnum = 0;
        createRandnum = true;
    }

    // 공격
    IEnumerator AttackDelay()
    {
        AudioSource sound;

        Anim.SetBool("Walk", false);
        s_Walk = false;

        yield return new WaitForSeconds(0.1f);

        while (1.2f > distance)
        {
            sound = RandomSound("dia_attack1", "dia_attack2", "dia_attack3");
            if (!isAttack)
            {
                break;
            }
            Attack(Anim, playerPos, Angle);
            s_Attack = true;
            sound.Play();
            yield return new WaitForSeconds(0.5f);
            Anim.SetBool("Attack", false);
            s_Attack = false;

            if (GetComponentInChildren<HitZone>().isHit && !death)
            {
                sound = RandomSound("hit1", "hit2", "hit3");
                sound.Play();
                if (Player_c.ManaShield)
                {
                    Player_s.mp -= AttackDmg;
                }
                else
                    Player_s.hp -= AttackDmg;
            }

            yield return new WaitForSeconds(1f);
        }

        yield return new WaitForSeconds(0.5f);

        isWalk = true;
        //if (Phase1)
        //    Randnum = 0;
        createRandnum = true;
        stillWalk = true;

    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (!death)
        {
            if (collision.gameObject.tag == "Skill_mfire")
            {
                if (!isHit)
                {
                    isHit = true;
                    StartCoroutine(HitDelay());
                    hp -= SkillDB.M_fire;
                    Debug.Log("hp: " + hp);
                }
            }

            if (collision.gameObject.tag == "Skill_blaze")
            {
                if (!isHit)
                {
                    isHit = true;
                    StartCoroutine(HitDelay());
                    hp -= SkillDB.Blaze;
                    Debug.Log("hp: " + hp);
                }
            }

            if (collision.gameObject.tag == "Skill_firewall")
            {
                if (!isHit)
                {
                    isHit = true;
                    StartCoroutine(HitDelay());
                    hp -= SkillDB.Firewall;
                    Debug.Log("hp: " + hp);
                }

            }

            if (collision.gameObject.tag == "Skill_icebolt")
            {
                if (!isHit)
                {
                    isHit = true;
                    StartCoroutine(HitDelay());
                }
                hp -= SkillDB.Icebolt;
                Debug.Log("hp: " + hp);
            }
        }
    }

    IEnumerator HitDelay()
    {
        AudioSource sound = RandomSound("dia_gethit1", "dia_gethit2", "dia_gethit3");
        sound.Play();
        yield return new WaitForSeconds(1f);
        isHit = false;
    }

    private void OnTriggerStay2D(Collider2D collision)
    {
         if (collision.gameObject.tag == "Layer_0")
        {
            gameObject.GetComponent<SpriteRenderer>().sortingOrder = 0;
            Shadow.GetComponent<SpriteRenderer>().sortingOrder = -1;
        }
        if (collision.gameObject.tag == "Layer_1")
        {
            gameObject.GetComponent<SpriteRenderer>().sortingOrder = 1;
            Shadow.GetComponent<SpriteRenderer>().sortingOrder = 0;
        }
        if (collision.gameObject.tag == "Layer_2")
        {
            gameObject.GetComponent<SpriteRenderer>().sortingOrder = 2;
            Shadow.GetComponent<SpriteRenderer>().sortingOrder = 1;
        }
        if (collision.gameObject.tag == "Layer_3")
        {
            gameObject.GetComponent<SpriteRenderer>().sortingOrder = 3;
            Shadow.GetComponent<SpriteRenderer>().sortingOrder = 2;
        }
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Layer_0" || collision.gameObject.tag == "Layer_1" ||
             collision.gameObject.tag == "Layer_2" || collision.gameObject.tag == "Layer_3" ||
             collision.gameObject.tag == "Layer_4")
        {
            gameObject.GetComponent<SpriteRenderer>().sortingOrder = 5;
            Shadow.GetComponent<SpriteRenderer>().sortingOrder = 4;
        }
    }


    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (!death)
        {
            if (collision.gameObject.tag == "Skill_meteor")
            {
                rigid.bodyType = RigidbodyType2D.Kinematic;
                rigid.useFullKinematicContacts = true;

                if (!isHit)
                {
                    isHit = true;
                    StartCoroutine(HitDelay());
                }

                hp -= SkillDB.Meteo;
                Debug.Log("hp: " + hp);
                rigid.bodyType = RigidbodyType2D.Dynamic;
            }

            if (collision.gameObject.tag == "Skill_icestorm")
            {
                rigid.bodyType = RigidbodyType2D.Kinematic;
                rigid.useFullKinematicContacts = true;

                if (!isHit)
                {
                    isHit = true;
                    StartCoroutine(HitDelay());
                }

                hp -= SkillDB.Icestorm;
                Debug.Log("hp: " + hp);
                rigid.bodyType = RigidbodyType2D.Dynamic;
            }

            if (collision.gameObject.tag == "Skill_blizzard")
            {
                rigid.bodyType = RigidbodyType2D.Kinematic;
                rigid.useFullKinematicContacts = true;

                if (!isHit)
                {
                    isHit = true;
                    StartCoroutine(HitDelay());
                }

                hp -= SkillDB.Blizzard;
                Debug.Log("hp: " + hp);
                rigid.bodyType = RigidbodyType2D.Dynamic;
            }

            if (collision.gameObject.tag == "Skill_fireball")
            {
                rigid.bodyType = RigidbodyType2D.Kinematic;
                rigid.useFullKinematicContacts = true;

                if (!isHit)
                {
                    isHit = true;
                    StartCoroutine(HitDelay());
                }

                hp -= SkillDB.Fireball;
                Debug.Log("hp: " + hp);
                rigid.bodyType = RigidbodyType2D.Dynamic;
            }

            if (collision.gameObject.tag == "Skill_firebolt")
            {
                rigid.bodyType = RigidbodyType2D.Kinematic;
                rigid.useFullKinematicContacts = true;

                if (!isHit)
                {
                    isHit = true;
                    StartCoroutine(HitDelay());
                }

                hp -= SkillDB.Firebolt;
                Debug.Log("hp: " + hp);
                rigid.bodyType = RigidbodyType2D.Dynamic;
            }
        }
    }
}
