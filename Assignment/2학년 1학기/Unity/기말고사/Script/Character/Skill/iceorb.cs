using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class iceorb : MonoBehaviour {
    public GameObject icebolt;
    public GameObject iceorbSfx;

    private float speed = 3.0f;

    public static int min_damage;
    public static int max_damage;
    [HideInInspector] public static int damage;

    bool isGenerate = false;

    public Vector2 mousePos;
    Vector2 thisObjPos;
    Vector2 direction;

    GameObject Sfx;

    // Use this for initialization
    void Start () {
        mousePos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
        thisObjPos = new Vector2(this.transform.position.x, this.transform.position.y);
        direction = (mousePos - thisObjPos).normalized;

        damage = Random.Range(min_damage, max_damage);

        Sfx = Instantiate(iceorbSfx) as GameObject;
        Sfx.transform.parent = this.transform;
    }
	
	// Update is called once per frame
	void Update () {
        this.transform.Translate(direction * speed * Time.deltaTime);

        if (transform.position.x == mousePos.x && transform.position.y == mousePos.y)
            Destroy(this.gameObject);
        Destroy(this.gameObject, 3.0f);

        if (!isGenerate)
        {
            isGenerate = true;
            StartCoroutine("IceBoltGenerate");
        }

        damage = Random.Range(min_damage, max_damage);
    }

    IEnumerator IceBoltGenerate()
    {
        int index = 8;
        float speed = 150.0f;

        int j = 0;
        while (j < 3)
        {
            yield return new WaitForSeconds(0.3f);
            for (int i = 0; i < index; i++)
            {
                GameObject item = (GameObject)Instantiate(icebolt, this.transform.position, Quaternion.identity);
                item.GetComponent<Rigidbody2D>().AddForce(new Vector2(speed * Mathf.Cos(Mathf.PI * 2 * i / index), speed * Mathf.Sin(Mathf.PI * i * 2 / index)));
                item.GetComponent<iceboltController>().Angle = 360 * i / index;
            }
            j++;
        }

        yield return new WaitForSeconds(0.5f);

        index = 30;
        for (int i = 0; i < index; i++)
        {
            GameObject item = (GameObject)Instantiate(icebolt, this.transform.position, Quaternion.identity);
            item.GetComponent<Rigidbody2D>().AddForce(new Vector2(speed * Mathf.Cos(Mathf.PI * 2 * i / index), speed * Mathf.Sin(Mathf.PI * i * 2 / index)));
            item.GetComponent<iceboltController>().Angle = 360 * i / index;
        }
    }
}
