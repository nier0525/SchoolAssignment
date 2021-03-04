using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SkillManager : MonoBehaviour {
    //사운드 오브젝트
    public GameObject blizzardSfx;
    public GameObject icecastSfx;
    public GameObject firecastSfx;
    public GameObject meteorlaunchSfx;
    public GameObject infernoloopSfx;

    //스킬 오브젝트
    public GameObject blizzard;         
    public GameObject icestorm;         
    public GameObject icecast;          
    public GameObject iceorb;           
    public GameObject manashield;      
    public GameObject manashieldCast;   
    public GameObject firecast;         
    public GameObject firestart;        
    public GameObject firewall_start;   
    public GameObject firebolt;         
    public GameObject fireball;         
    public GameObject meteor_target;    

    Vector2 mousePos;
    Vector2 playerPos;

    float x1, x2;
    float y1, y2;

    //*********************************************************************************
    //스킬 사용
    //*********************************************************************************

    //마나쉴드
    public void UseManaShield(Vector2 playerPos)
    {
        GameObject cast = Instantiate(manashieldCast) as GameObject;
        GameObject item = Instantiate(manashield) as GameObject;
        float x = playerPos.x;
        float y = playerPos.y + 0.5f;
        item.transform.position = new Vector2(x, y);
        item.transform.parent = this.transform;
        cast.transform.position = this.transform.position;
    }

    //메테오
    public void UseMeteor(Vector2 mousePos)
    {
        FireCast();
        GameObject item = Instantiate(meteorlaunchSfx) as GameObject;
        MeteorTarget(mousePos);
        Destroy(item.gameObject, 3.2f);
    }

    //파이어볼
    public void UseFireball()
    {
        FireCast();
        Fireball();
    }

    //파이어볼트
    public void UseFirebolt()
    {
        FireCast();
        Firebolt();
    }

    //블리자드
    public void UseBlizzard(Vector2 mousePos, Vector2 playerPos)
    {
        this.playerPos = playerPos;
        this.mousePos = mousePos;
        StartCoroutine("Blizzard");
    }

    //아이스볼트
    public void UseIceBolt(Vector2 mousePos, Vector2 playerPos)
    {
        GameObject icecast_Sfx = Instantiate(icecastSfx) as GameObject;
        IceCast(playerPos);
        IceOrb(mousePos, playerPos);
        Destroy(icecast_Sfx.gameObject,1.0f);
    }

    //블레이즈
    public void UseBlaze()
    {
        FireCast();
        GameObject item = Instantiate(infernoloopSfx) as GameObject;
        StartCoroutine("Blaze");
        Destroy(item.gameObject, 16.3f);
    }

    //파이어월
    public void UseFireWall(Vector2 mousePos)
    {
        this.mousePos = mousePos;
        x1 = this.mousePos.x;
        x2 = x1;
        y1 = this.mousePos.y;
        y2 = y1;
        FireCast();
        GameObject item = Instantiate(infernoloopSfx) as GameObject;
        StartCoroutine("Firewall");
        Destroy(item.gameObject, 11.3f);
    }

    //*********************************************************************************
    //스킬 프리팹 인스턴시에이트
    //*********************************************************************************

    //메테오 타겟
    public void MeteorTarget(Vector2 mousePos)
    {
        float x = mousePos.x;
        float y = mousePos.y;
        GameObject item = Instantiate(meteor_target) as GameObject;
        item.transform.position = new Vector2(x, y);
    }

    //파이어볼
    public void Fireball()
    {
        GameObject item = Instantiate(fireball) as GameObject;
        item.transform.position = this.transform.position;
    }

    //파이어볼트
    public void Firebolt()
    {
        GameObject item = Instantiate(firebolt) as GameObject;
        item.transform.position = this.transform.position;
    }

    //아이스오브
    public void IceOrb(Vector2 mousePos, Vector2 playerPos)
    {
        GameObject item = Instantiate(iceorb) as GameObject;
        float x = playerPos.x;
        float y = playerPos.y;
        item.transform.position = new Vector2(x, y);
    }

    //아이스 캐스팅
    public void IceCast(Vector2 playerPos)
    {
        GameObject item = Instantiate(icecast) as GameObject;
        item.transform.position = new Vector2(playerPos.x, playerPos.y + 0.1f);
        item.transform.parent = this.transform;
    }

    //파이어월 파이어생성
    public void Firewall_create(float x, float y,bool start)
    {
        GameObject item = Instantiate(firewall_start) as GameObject;
        if (start)
        {
            this.x1 = x;
            this.x2 = x;
            this.y1 = y;
            this.y2 = y;
        }
        item.transform.position = new Vector2(x, y);
    }

    //블레이즈 파이어생성
    public void Fire_start()
    {
        GameObject item = Instantiate(firestart) as GameObject;
        item.transform.position = this.transform.position;
    }

    //파이어 캐스팅
    public void FireCast()
    {
        GameObject firecast_Sfx = Instantiate(firecastSfx) as GameObject;
        GameObject item = Instantiate(firecast) as GameObject;
        item.transform.position = this.transform.position;
        item.transform.parent = this.transform;
        Destroy(firecast_Sfx.gameObject, 1.0f);
    }

    //아이스스톰, 블리자드 생성
    private void CreateObj(GameObject obj, Vector2 mousePos)
    {
        GameObject item = Instantiate(obj) as GameObject;
        float x = mousePos.x + Random.Range(-1.3f, 1.3f);
        float y = mousePos.y + Random.Range(1.7f, 2.0f);
        item.transform.position = new Vector2(x, y);
    }

    //*********************************************************************************
    //반복 스킬 코루틴
    //*********************************************************************************

    IEnumerator Blizzard()
    {
        GameObject bSfx = Instantiate(blizzardSfx) as GameObject;
        IceCast(playerPos);
        for (int i = 0; i < 10; i++)
        {
            CreateObj(blizzard, mousePos);
            yield return new WaitForSeconds(0.3f);
            CreateObj(icestorm, mousePos);
            yield return new WaitForSeconds(0.3f);
        }
        yield return new WaitForSeconds(1.0f);
        Destroy(bSfx.gameObject);
    }

    IEnumerator Blaze()
    {
        for (int i = 0; i < 50; i++)
        {
            Fire_start();
            yield return new WaitForSeconds(0.1f);
        }
    }

    IEnumerator Firewall()
    {
        float Angle = GetDegree(transform.position, mousePos);

        Firewall_create(mousePos.x, mousePos.y + 0.7f, true);
        for (int i=0;i<10;i++)
        {
            SetFirewallPos(Angle);

            yield return new WaitForSeconds(0.15f);
            Firewall_create(x1, y1, false);
            Firewall_create(x2, y2, false);
        }
    }

    //파이어월 좌표 지정
    private void SetFirewallPos(float Angle)
    {
        if ((337.5 <= Angle && Angle <= 360 ) || (0<=Angle && Angle < 22.5) || (157.5 <= Angle && Angle < 202.5))
        {
            y1 += 0.4f;
            y2 -= 0.4f;
            return;
        }
        else if ((22.5 <= Angle && Angle < 67.5) || (202.5 <= Angle && Angle < 247.5))
        {
            x1 -= 0.2f;
            y1 += 0.2f;
            x2 += 0.2f;
            y2 -= 0.2f;
            return;
        }
        else if ((67.5 <= Angle && Angle < 112.5) || (247.5 <= Angle && Angle < 292.5))
        {
            x1 -= 0.4f;
            x2 += 0.4f;
            return;
        }
        else if ((112.5 <= Angle && Angle < 157.5) || (292.5 <= Angle && Angle < 337.5))
        {
            x1 -= 0.2f;
            y1 -= 0.2f;
            x2 += 0.2f;
            y2 += 0.2f;
            return;
        }
    }

    private float GetDegree(Vector2 playerPos, Vector2 mousePos)
    {
        float angle = Mathf.Atan2(mousePos.y - playerPos.y, mousePos.x - playerPos.x) * 180 / Mathf.PI;
        if (angle < 0) angle += 360;

        return angle;
    }
}
