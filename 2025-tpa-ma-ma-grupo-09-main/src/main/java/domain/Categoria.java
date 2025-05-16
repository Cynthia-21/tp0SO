package domain;

import lombok.Getter;

@Getter
public class Categoria {
  private String nombre;

  public Categoria(String nombre) {
    this.nombre = nombre;
  }
}
