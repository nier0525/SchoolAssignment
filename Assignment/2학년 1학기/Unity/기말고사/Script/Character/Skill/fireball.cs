using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class fireball : MonoBehaviour {
    public GameObject hit;
    public GameObject fireballSfx;
    private Animator animator;
    private float speed = 4.0f;


    public static int min_damage;
    public static int max_damage;
    [HideInInspector] public static int damage;

    Vector2 mousePos;
    Vector2 thisObjPos;
    Vector2 direction;

    GameObject Sfx;

    // Use this for initialization
    void Start () {
        animator = GetComponent<Animator>();
        mousePos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
        thisObjPos = new Vector2(this.transform.position.x, this.transform.position.y);
        animator.SetFloat("Angle", GetDegree(transform.position, mousePos));
        direction = (mousePos - thisObjPos).normalized;

        Sfx = Instantiate(fireballSfx) as GameObject;
        Sfx.transform.parent = this.transform;

        damage = Random.Range(min_damage, max_damage);
        Debug.Log(damage);
    }

    // Update is called once per frame
    void Update () {

        this.transform.Translate(direction * speed * Time.deltaTime);

        Destroy(this.gameObject, 3.0f);
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.tag == "Monster")
        {
            GameObject item = Instantiate(hit) as GameObject;
            item.transform.position = this.transform.position;
            Destroy(this.gameObject);
        }
    }

    private float GetDegree(Vector2 playerPos, Vector2 mousePos)
    {
        float angle = Mathf.Atan2(mousePos.y - playerPos.y, mousePos.x - playerPos.x) * 180 / Mathf.PI;
        if (angle < 0) angle += 360;

        return angle;
    }
}
