using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class meteor_target : MonoBehaviour {
    public GameObject meteor;

    private bool create;

	// Use this for initialization
	void Start () {
        create = true;
	}
	
	// Update is called once per frame
	void Update () {
        if(create)
        {
            GameObject item = Instantiate(meteor) as GameObject;
            item.transform.position = new Vector2(this.transform.position.x - 2, this.transform.position.y + 16);
            create = false;
        }
        Destroy(this.gameObject, 3.3f);
	}
}
