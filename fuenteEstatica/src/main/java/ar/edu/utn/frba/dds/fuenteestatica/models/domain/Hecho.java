package ar.edu.utn.frba.dds.fuenteestatica.models.domain;

import ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes.estrategiasFuenteEstatica.Archivo;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.origen.Origen;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.ubicacion.Ubicacion;
import lombok.Getter;
import lombok.Setter;

import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;



@Getter
@Setter
public class Hecho {
  private String id;
  private String titulo;
  private String descripcion;
  private List<Categoria> categorias;
  private Ubicacion ubicacion;
  private LocalDate fechaAcontecimiento;
  private LocalDate fechaDeCarga;
  private Origen origen;
  private Boolean estaEliminado;
  private List<Etiqueta> etiquetas;
  private Archivo archivoOrigen;

  public Hecho(String id, String titulo) {
    this.id = id;
    this.titulo = titulo;
    this.categorias = new ArrayList<>();
    this.fechaDeCarga = LocalDate.now();
    this.estaEliminado = false;
    this.etiquetas = new ArrayList<>();
  }

  public Hecho(String titulo) {
    this.titulo = titulo;
    this.categorias = new ArrayList<>();
    this.fechaDeCarga = LocalDate.now();
    this.estaEliminado = false;
    this.etiquetas = new ArrayList<>();
  }


  public boolean contieneTexto(String texto) {
    return titulo.toLowerCase().contains(texto.toLowerCase())
        || descripcion.toLowerCase().contains(texto.toLowerCase());
  }


  public void agregarCategoria(Categoria categoria) {
    if (!this.categorias.contains(categoria)) {
      this.categorias.add(categoria);
    }
  }

  public void agregarEtiqueta(Etiqueta etiqueta) {
    if (!this.etiquetas.contains(etiqueta)) {
      this.etiquetas.add(etiqueta);
    }
  }


  //Para un print
  @Override
  public String toString() {
    return "Hecho { \n\t"
        + "titulo = '" + titulo + "\n\t"
        + "categorias = " + categorias.stream().map(Categoria::getNombre).toList() + "\n\t"
        + "ubicacion = " + ubicacion + "\n\t"
        + "fecha = " + fechaAcontecimiento + "\n\t"
        + "origen = " + origen + "\n\t"
        + "etiquetas = " + etiquetas + "\n}"
        + "}\n";
  }
}
