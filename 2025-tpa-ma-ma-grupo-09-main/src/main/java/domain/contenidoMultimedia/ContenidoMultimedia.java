package domain.contenidoMultimedia;

import lombok.Setter;

@Setter
public class ContenidoMultimedia {
  String path;
  String descripcion;
  TipoContenido tipoContenido;

  public ContenidoMultimedia(String descripcion) {
    this.path = "";
    this.descripcion = descripcion;
    this.tipoContenido = null;
  }
}