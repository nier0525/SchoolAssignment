using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.UI;

public class TextAnime : MonoBehaviour
{
    string origin = "";
    public int TextCount;

    // Start is called before the first frame update
    void Start()
    {
        TextCount = 0;
    }

    public void TextTyping(/*string text*/)
    {
        origin = gameObject.GetComponent<Text>().text;

        if (origin.Length > 0)
        {
            gameObject.GetComponent<Text>().text = "";
            gameObject.GetComponent<Text>().text += origin[TextCount++];

            StartCoroutine("Typing");
        }
    }

    IEnumerator Typing()
    {
        yield return new WaitForSeconds(0.1f);

        if (origin.Length - TextCount > 0)
        {
            if (Input.GetMouseButtonDown(0) || Input.GetKeyDown(KeyCode.Space))
            {
                TextCount = origin.Length;
            }

            gameObject.GetComponent<Text>().text += origin[TextCount++];
            StartCoroutine("Typing");
        }

        yield return true;
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
