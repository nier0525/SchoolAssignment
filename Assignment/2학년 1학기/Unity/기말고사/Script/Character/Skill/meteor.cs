using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class meteor : MonoBehaviour {
    public GameObject hit;
    public GameObject fire;
    public GameObject meteorimpactSfx;

    float times = 0.0f;

    public static int min_damage;
    public static int max_damage;
    public static int damage;

    // Use this for initialization
    void Start () {
        damage = Random.Range(min_damage, max_damage);
        Debug.Log(damage);
	}
	
	// Update is called once per frame
	void Update () {
        transform.position -= new Vector3(-0.6f * Time.deltaTime, 5.0f * Time.deltaTime, 0);

        times += Time.deltaTime;

        if(times>3.1f)
        {
            GameObject impactSfx = Instantiate(meteorimpactSfx) as GameObject;
            Destroy(impactSfx.gameObject, 2);

            float x = this.transform.position.x;
            float y = this.transform.position.y;
            GameObject hit1 = Instantiate(hit) as GameObject;
            hit1.transform.position = this.transform.position;
            GameObject fire1 = Instantiate(fire) as GameObject;
            fire1.transform.position = this.transform.position;

            GameObject hit2 = Instantiate(hit) as GameObject;
            hit2.transform.position = new Vector2(x, y+0.5f);
            GameObject fire2 = Instantiate(fire) as GameObject;
            fire2.transform.position = new Vector2(x, y+0.5f);

            GameObject hit3 = Instantiate(hit) as GameObject;
            hit3.transform.position = new Vector2(x, y-0.4f);
            GameObject fire3 = Instantiate(fire) as GameObject;
            fire3.transform.position = new Vector2(x, y-0.4f);

            GameObject hit4 = Instantiate(hit) as GameObject;
            hit4.transform.position = new Vector2(x-0.2f, y-0.8f);
            GameObject fire4 = Instantiate(fire) as GameObject;
            fire4.transform.position = new Vector2(x-0.2f, y-0.8f);

            GameObject hit5 = Instantiate(hit) as GameObject;
            hit5.transform.position = new Vector2(x-0.2f, y+0.2f);
            GameObject fire5 = Instantiate(fire) as GameObject;
            fire5.transform.position = new Vector2(x-0.2f, y+0.2f);

            GameObject hit6 = Instantiate(hit) as GameObject;
            hit6.transform.position = new Vector2(x-0.4f, y+0.4f);
            GameObject fire6 = Instantiate(fire) as GameObject;
            fire6.transform.position = new Vector2(x-0.4f, y+0.4f);

            GameObject hit7 = Instantiate(hit) as GameObject;
            hit7.transform.position = new Vector2(x - 0.7f, y - 0.5f);
            GameObject fire7 = Instantiate(fire) as GameObject;
            fire7.transform.position = new Vector2(x - 0.7f, y - 0.5f);

            GameObject hit8 = Instantiate(hit) as GameObject;
            hit8.transform.position = new Vector2(x - 0.4f, y - 0.4f);
            GameObject fire8 = Instantiate(fire) as GameObject;
            fire8.transform.position = new Vector2(x - 0.4f, y - 0.4f);

            GameObject hit9 = Instantiate(hit) as GameObject;
            hit9.transform.position = new Vector2(x - 0.8f, y - 0.4f);
            GameObject fire9 = Instantiate(fire) as GameObject;
            fire9.transform.position = new Vector2(x - 0.8f, y - 0.4f);

            GameObject hit10 = Instantiate(hit) as GameObject;
            hit10.transform.position = new Vector2(x + 0.4f, y - 0.3f);
            GameObject fire10 = Instantiate(fire) as GameObject;
            fire10.transform.position = new Vector2(x + 0.4f, y - 0.3f);

            GameObject hit11 = Instantiate(hit) as GameObject;
            hit11.transform.position = new Vector2(x + 0.5f, y - 0.1f);
            GameObject fire11 = Instantiate(fire) as GameObject;
            fire11.transform.position = new Vector2(x + 0.5f, y - 0.1f);

            GameObject hit12 = Instantiate(hit) as GameObject;
            hit12.transform.position = new Vector2(x + 0.7f, y - 0.2f);
            GameObject fire12 = Instantiate(fire) as GameObject;
            fire12.transform.position = new Vector2(x + 0.7f, y - 0.2f);

            GameObject hit13 = Instantiate(hit) as GameObject;
            hit13.transform.position = new Vector2(x + 0.8f, y - 0.5f);
            GameObject fire13 = Instantiate(fire) as GameObject;
            fire13.transform.position = new Vector2(x + 0.8f, y - 0.5f);

            GameObject hit14 = Instantiate(hit) as GameObject;
            hit14.transform.position = new Vector2(x + 0.8f, y + 0.3f);
            GameObject fire14 = Instantiate(fire) as GameObject;
            fire14.transform.position = new Vector2(x + 0.8f, y + 0.3f);

            GameObject hit15 = Instantiate(hit) as GameObject;
            hit15.transform.position = new Vector2(x - 0.8f, y + 0.2f);
            GameObject fire15 = Instantiate(fire) as GameObject;
            fire15.transform.position = new Vector2(x - 0.8f, y + 0.2f);

            GameObject hit16 = Instantiate(hit) as GameObject;
            hit16.transform.position = new Vector2(x - 1.0f, y);
            GameObject fire16 = Instantiate(fire) as GameObject;
            fire16.transform.position = new Vector2(x - 1.0f, y);

            GameObject hit17 = Instantiate(hit) as GameObject;
            hit17.transform.position = new Vector2(x + 1.0f, y - 0.7f);
            GameObject fire17 = Instantiate(fire) as GameObject;
            fire17.transform.position = new Vector2(x + 1.0f, y - 0.7f);

            GameObject hit18 = Instantiate(hit) as GameObject;
            hit18.transform.position = new Vector2(x + 0.3f, y - 0.6f);
            GameObject fire18 = Instantiate(fire) as GameObject;
            fire18.transform.position = new Vector2(x + 0.3f, y - 0.6f);

            GameObject hit19 = Instantiate(hit) as GameObject;
            hit19.transform.position = new Vector2(x + 1.2f, y - 0.2f);
            GameObject fire19 = Instantiate(fire) as GameObject;
            fire19.transform.position = new Vector2(x + 1.2f, y - 0.2f);
            Destroy(this.gameObject);
        }
    }
}
